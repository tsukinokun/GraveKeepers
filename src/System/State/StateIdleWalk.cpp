//---------------------------------------------------------------------------
//!	@file	StateIdleWalk.cpp
//! @brief	歩き&待機状態コンポーネント
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "StateIdleWalk.h"
#include <System/State/StateDeath.h>
#include <System/Component/ComponentSpringArm.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentStatus.h>
#include <System/Component/ComponentLiftable.h>
#include <System/Component/ComponentJump.h>
#include <System/State/StateJump.h>
void StateIdleWalk::Init()
{
    __super::Init();
    auto owner = GetOwner();
    prev_pos_  = owner->GetTranslate();    //初期座標を設定しておく
}

void StateIdleWalk::Update()
{
    __super::Update();

    auto owner = GetOwner();

    //アニメーション
    auto   model    = owner->GetComponent<ComponentModel>();
    float3 curr_pos = owner->GetTranslate();    //現在の座標
    //前フレームとの差分ベクトルを求める
    float3 diff = curr_pos - prev_pos_;
    //---------------------------------------------------------------------------
    // 持ち上げられ中なら、idleにして、モデルも回転
    //---------------------------------------------------------------------------
    if(auto liftable_comp = owner->GetComponent<ComponentLiftable>()) {
        if(liftable_comp->IsLifted()) {
            model->PlayAnimationNoSame("idle", true);
            model->SetRotationAxisXYZ(float3(0.0f, 0.0f, 90.0f));    //回転
        }
        else {
            IdleWalkChange(model, diff);
        }
    }
    else {
        //---------------------------------------------------------------------------
        // 通常処理
        //---------------------------------------------------------------------------
        IdleWalkChange(model, diff);
    }
    prev_pos_ = curr_pos;    //updateの末尾で、座標を保存しておく

    //---------------------------------------------------------------------------
    // HPが0で死亡
    //---------------------------------------------------------------------------
    if(auto status = owner->GetComponent<ComponentStatus>()) {
        if(status->IsDead()) {
            ChangeState<StateDeath>();
        }
    }
    //---------------------------------------------------------------------------
    // ジャンプしたフレームであるなら、ジャンプ状態へ
    //---------------------------------------------------------------------------
    if(auto jump_comp = owner->GetComponent<ComponentJump>()) {
        //ジャンプしたフレームであるなら
        if(jump_comp->IsJumpFrame()) {
            ChangeState<StateJump>();    //ジャンプ状態へ
        }
    }
}

//--------------------------------------------------------------------
// 通常時の待機モーションと歩行モーションの切り替え処理
//--------------------------------------------------------------------
void StateIdleWalk::IdleWalkChange(std::shared_ptr<ComponentModel> model, float3 diff)
{
    //---------------------------------------------------------------------------
    // 通常処理
    //---------------------------------------------------------------------------
    model->SetRotationAxisXYZ(float3(0.0f, 180.0f, 0.0f));    //回転
    //前フレームから移動しているなら歩き状態
    if(float1(0.01f) < dot(diff, diff)) {
        model->PlayAnimationNoSame("walk", true);
    }
    else {
        model->PlayAnimationNoSame("idle", true);
    }
}

void StateIdleWalk::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"StateIdleWalk")) {
            // GUI上でオーナーから自分(SampleObjectController)を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

CEREAL_REGISTER_TYPE(StateIdleWalk)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, StateIdleWalk)
