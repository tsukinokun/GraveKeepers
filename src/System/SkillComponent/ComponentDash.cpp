//---------------------------------------------------------------------------
//!	@file	ComponentDash.cpp
//! @brief	突進のコンポーネント
//---------------------------------------------------------------------------
#include "ComponentDash.h"
#include <Game/Scene/SkillObject/Dash.h>
#include <System/Component/ComponentEffect.h>
#include <System/Component/ComponentRigidbody.h>
#include <System/Component/ComponentStatus.h>
#include <System/Component/ComponentHitInfo.h>
#include <cmath>
#include <Game/System/SoundBuffer.h>

//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentDash::Init()
{    // 初期化処理
    __super::Init();
}

//---------------------------------------------------------------------------
//! @brief	更新処理関数
//---------------------------------------------------------------------------
void ComponentDash::Update()
{    // 初期化処理
    __super::Update();
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentDash::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"突進コンポーネント")) {
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

//---------------------------------------------------------------------------
//! @brief	スキルを発動する
//---------------------------------------------------------------------------
std::shared_ptr<ComponentSkill> ComponentDash::UseSkill()
{
    __super::UseSkill();

    //サウンドの再生
    int handle = SoundBuffer::GetSoundHandle("rush");
    if(handle != -1) {
        PlaySoundMem(handle, DX_PLAYTYPE_BACK);
    }

    auto owner = GetOwner();

    //---------------------------------------------------------------------------
    // スキルオブジェクトの生成
    //---------------------------------------------------------------------------
    auto dash = Scene::Object::Create<Dash>();    //突進オブジェクトを生成
    dash->SetSkillOwnerName(owner->GetName());    //スキル使用者の名前を

    //高さの値は仮、後に持ち上げているオブジェクトの位置に変更するはず。
    const float3 pos = owner->GetTranslate() + float3(0.0f, 10.0f, 0.0f);
    dash->SetTranslate(pos);
    //大きさを変える
    dash->SetScaleAxisXYZ(SCALE_);
    //プレイヤーの向きに合わせる
    dash->SetRotationAxisXYZ(owner_->GetRotationAxisXYZ());

    //---------------------------------------------------------------------------
    // 物理的な突進（プレイヤー本体を前方に押し出す）
    //---------------------------------------------------------------------------
    // 前方ベクトルの計算（yaw が degree の想定）
    constexpr float kdeg_to_rad = 3.14159265358979323846f / 180.0f;
    float           yaw_deg     = owner->GetRotationAxisXYZ().y;
    float           yaw_rad     = yaw_deg * kdeg_to_rad;
    float3          forward     = float3(-sinf(yaw_rad), 0.0f, -cosf(yaw_rad));    // 他の箇所で使われている符号系に合わせる

    // 強さ
    const float dash_strength = 30.0f;
    //持続時間
    const float dash_duration = 3.0f;    // 秒

    if(auto rb = owner->GetComponent<ComponentRigidbody>()) {
        rb->SetVelocity(float3(0.0f, 0.0f, 0.0f));    // 既存速度をリセットして瞬間的加速にする
        rb->AddImpulse(forward * -dash_strength);     // 前方に押し出す
    }

    //---------------------------------------------------------------------------
    // 衝突時のダメージ・ノックバック処理をこの Component に設定（有効時間中のみ）
    //---------------------------------------------------------------------------
    // 自身の shared_ptr / weak_ptr を準備
    auto                         self_sp = std::dynamic_pointer_cast<ComponentDash>(shared_from_this());
    std::weak_ptr<ComponentDash> self_wp = self_sp;

    // 既存のコールバックを保存しておき、終了時に復帰させる
    auto prev_onhit = self_sp->OnHitComponentFunc;

    // ダメージ・ノックバック
    const int   hit_damage         = 30;
    const float hit_force_strength = 50.0f;

    // オーナーと安全に参照するための weak_ptr
    auto                  owner_sp = owner->shared_from_this();
    std::weak_ptr<Object> owner_wp = owner_sp;

    // コールバックを設定
    self_sp->OnHitComponentFunc = [self_wp, owner_wp, hit_damage, hit_force_strength](const HitInfo& hit_info) {
        // 安全参照の復元
        auto self  = self_wp.lock();
        auto owner = owner_wp.lock();
        if(!self || !owner)
            return;

        // hit_info から当たった相手のオーナーを取得（当たり判定側のコリジョンが持つオーナー）
        if(!hit_info.hit_collision_)
            return;

        auto other_owner = hit_info.hit_collision_->GetOwner();
        if(!other_owner)
            return;

        // 自分自身には当たらないようにする
        if(other_owner->GetName() == owner->GetName())
            return;

        //ステータスコンポーネントを取得
        if(auto status_comp = other_owner->GetComponent<ComponentStatus>()) {
            // ダメージ処理
            status_comp->TakeDamage(hit_damage);
        }

        // ノックバック（当たった相手の剛体があれば力を加える）
        if(auto other_rb = other_owner->GetComponent<ComponentRigidbody>()) {
            // 自分から相手への方向
            float3 direction = other_owner->GetTranslate() - owner->GetTranslate();
            direction        = normalize(direction);
            other_rb->SetVelocity(float3(0.0f, 0.0f, 0.0f));    // 速度をリセット
            other_rb->AddImpulse(direction * hit_force_strength);
        }
    };

    //---------------------------------------------------------------------------
    // 追従処理：dashオブジェクトを常にオーナーの位置に更新する
    //---------------------------------------------------------------------------
    auto owner_wp_follow = owner_wp;    // ラムダ用にコピー
    auto follow_proc     = [dash, owner_wp_follow]() {
        if(auto owner = owner_wp_follow.lock()) {
            // オーナーの少し上にエフェクトを出す（高さ調整は適宜）
            const float3 offset(0.0f, 1.0f, 0.0f);
            dash->SetTranslate(owner->GetTranslate() + offset);

            // 回転も同期させる場合
            dash->SetRotationAxisXYZ(owner->GetRotationAxisXYZ());
        }
    };
    // 毎フレームUpdateのタイミングで実行
    dash->SetProc("dash_follow_owner", follow_proc, ProcTiming::Update, ProcPriority::LOWEST);

    //---------------------------------------------------------------------------
    // ダッシュ有効時間の管理（dash オブジェクトが生存している間にタイマーで解除）
    //---------------------------------------------------------------------------
    auto timer     = std::make_shared<float>(0.0f);
    auto dash_proc = [dash, timer, dash_duration, self_wp, prev_onhit]() mutable {
        *timer += GetDeltaTime();    // フレーム毎に経過時間を加算
        if(*timer >= dash_duration) {
            // スキル時間終了：Component に設定した OnHitComponentFunc を元に戻す
            if(auto self = self_wp.lock()) {
                self->OnHitComponentFunc = prev_onhit;
            }

            // 念のため follow 用プロシージャ名を解除（存在していても安全）
            dash->ResetProc("dash_follow_owner");

            // エフェクト用オブジェクトを破棄
            Scene::Object::Release(dash);
        }
    };
    dash->SetProc("dash_timer", dash_proc, ProcTiming::Update, ProcPriority::NORMAL);

    //自身のポインタを変換
    return dynamic_pointer_cast<ComponentSkill>(shared_from_this());
}

CEREAL_REGISTER_TYPE(ComponentDash)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentDash)
