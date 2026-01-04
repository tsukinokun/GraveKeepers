//---------------------------------------------------------------------------
//!	@file	ComponentDash.cpp
//! @brief	突進のコンポーネント
//---------------------------------------------------------------------------
#include "ComponentDash.h"
#include <Game/Scene/SkillObject/Dash.h>
#include <System/Component/ComponentEffect.h>
#include <System/Component/ComponentRigidbody.h>
#include <cmath>

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
    //スキル使用条件を満たしたら
    if(is_use_skill_()) {
        UseSkill();    // スキルを使用する
    }
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
    // エフェクトをワールド固定で生成（Dash オブジェクトの元からついているエフェクトは削除）
    //---------------------------------------------------------------------------
    {
        const std::string eff_path = "data/PoyPoy/Effect/Dash/Simple_SpawnMethod1.efkefc";
        // ワールド固定のエフェクトオブジェクトを生成（pos に固定）
        ComponentEffect::Object::Create(eff_path, pos, float3(0.0f), float3(1.0f));
        // Dash に付与されている ComponentEffect があれば削除して追従を止める
        if(dash->GetComponent<ComponentEffect>()) {
            dash->RemoveComponent<ComponentEffect>();
        }
    }

    //---------------------------------------------------------------------------
    // 使用者に追従させる処理（当たり判定は使用者に追従）
    //---------------------------------------------------------------------------
    {
        auto owner_sp    = owner;
        auto follow_proc = [dash, owner_sp]() {
            if(owner_sp) {
                // 常にオーナーのワールド位置 + オフセットに追従（当たり判定を使用者に合わせる）
                dash->SetTranslate(owner_sp->GetTranslate() + float3(0.0f, 10.0f, 0.0f));
            }
            else {
                // オーナーが存在しなくなったらプロシージャ解除
                dash->ResetProc("dash_follow_owner");
            }
        };
        dash->SetProc("dash_follow_owner", follow_proc, ProcTiming::Update, ProcPriority::NORMAL);
    }

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
    // ダッシュオブジェクトの寿命管理（一定時間で自動削除）
    auto timer     = std::make_shared<float>(0.0f);
    auto dash_proc = [dash, timer, dash_duration]() mutable {
        *timer += GetDeltaTime();    // 1秒間に 1.0 加算される関数
        if(*timer >= dash_duration) {
            // 解放前に follow プロシージャも解除しておく
            dash->ResetProc("dash_follow_owner");
            Scene::Object::Release(dash);
        }
    };
    dash->SetProc("dash_timer", dash_proc, ProcTiming::Update, ProcPriority::NORMAL);

    //自身のポインタを変換
    return dynamic_pointer_cast<ComponentSkill>(shared_from_this());
}

CEREAL_REGISTER_TYPE(ComponentDash)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentDash)
