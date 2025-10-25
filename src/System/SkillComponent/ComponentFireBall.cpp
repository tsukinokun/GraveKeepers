//---------------------------------------------------------------------------
//!	@file	ComponentFireBall.cpp
//! @brief	ファイアボールのコンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentFireBall.h"
#include <System/Component/ComponentEffect.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentRigidbody.h>

//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentFireBall::Init()
{    // 初期化処理
    __super::Init();
}

//---------------------------------------------------------------------------
//! @brief	更新処理関数
//---------------------------------------------------------------------------
void ComponentFireBall::Update()
{    // 初期化処理
    __super::Update();
    //スキル使用条件を満たしたら
    if(is_use_skill_()) {
        UseSkill();    // スキルを使用する
    }

    //エフェクトが再生されているとき
    if(is_effect_play_) {
        auto col      = effect_->GetComponent<ComponentCollisionSphere>();    //コリジョンを取得
        auto col_size = col->GetRadius();                                     //コリジョンの大きさを取得

        //エフェクトのコンポーネントを取得
        if(auto effect = effect_->GetComponent<ComponentEffect>()) {
            auto time = effect->GetEffectTime();    //エフェクトの再生時間を取得

            //エフェクトの再生時間が2秒を超えたら
            if(time > COL_RESIZE_TIME_) {
                col_size += COL_CHANGE_SPEED_;    //コリジョンの大きさを徐々に大きくする

                //コリジョンの大きさが上限を超えたら
                if(col_size > COL_SIZE_MAX_) {
                    col_size = COL_SIZE_MAX_;    //コリジョンの大きさの上限
                }

                col->SetRadius(col_size);     //コリジョンの大きさを設定
                col->UseGravity(false);       //重力を使用しない
                col->SetEnableFlag(false);    //コリジョンを無効にする
            }

            //エフェクトの再生時間が4秒を超えたら
            if(time > COL_REMOVE_TIME_) {
                effect_->RemoveComponent<ComponentCollisionSphere>();    //エフェクトを停止する
                effect_->RemoveComponent<ComponentRigidbody>();          //リジッドボディを削除する
                is_effect_play_ = false;                                 //エフェクトが再生されない状態へ
            }
        }
    }
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentFireBall::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"ファイアボールコンポーネント")) {
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
std::shared_ptr<ComponentSkill> ComponentFireBall::UseSkill()
{
    __super::UseSkill();
    auto owner = GetOwner();
    //ファイルパス
    const std::string eff_name = "data/PoyPoy/Effect/Fireball/FireBall3.efkefc";
    //高さの値は仮、後に持ち上げているオブジェクトの位置に変更するはず。
    const float3 pos = owner->GetTranslate() + float3(0.0f, 20.0f, 0.0f);
    effect_          = ComponentEffect::Object::Create(eff_name, pos);

    effect_->GetComponent<ComponentEffect>()->SetPlaySpeed(EFFECT_PLAY_SPEED_);

    //プレイヤーの向きに合わせる
    effect_->SetRotationAxisXYZ(owner_->GetRotationAxisXYZ() * -1.0f);

    is_effect_play_ = true;    //エフェクトが再生されている
    MoveCollision();           //コリジョンを移動させる関数

    //自身のポインタを変換
    return dynamic_pointer_cast<ComponentSkill>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief	スキルのコリジョンを移動させる関数
//! @auther 田中南々子
//---------------------------------------------------------------------------

void ComponentFireBall::MoveCollision()
{
    //コリジョンの位置
    effect_->AddComponent<ComponentCollisionSphere>()->SetName(u8"FireBall");

    auto col  = effect_->GetComponent<ComponentCollisionSphere>();    //コリジョンを取得
    auto body = effect_->AddComponent<ComponentRigidbody>();          //リジッドボディを追加

    float3 throw_impulse  = float3(0.0f, THROW_VIRTICAL_POWER_, 0.0f);    //投げる力の初期化
    float3 owner_rot      = GetOwner()->GetRotationAxisXYZ();             //オーナーの向きを取得
    owner_rot.y          += 180.0f;                                       //座標系の関係でyを180度回転する、オブジェクトの背中が正面

    //オーナーのy軸回転から、throw_impulse_のxとzを設定
    throw_impulse.x = -THROW_HORIZONTAL_POWER_ * sinf(D2R(owner_rot.y));
    throw_impulse.z = -THROW_HORIZONTAL_POWER_ * cosf(D2R(owner_rot.y));

    body->AddImpulse(throw_impulse);    //投げる力を加える
    col->SetEnableFlag(true);           //コリジョンを有効にする
    col->UseGravity();                  //重力を使用する
}

CEREAL_REGISTER_TYPE(ComponentFireBall)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentFireBall)
