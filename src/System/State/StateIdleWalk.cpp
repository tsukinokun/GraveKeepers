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
    auto model = owner->GetComponent<ComponentModel>();

    float3 curr_pos = owner->GetTranslate();    //現在の座標
    //前フレームとの差分ベクトルを求める
    float3 diff = curr_pos - prev_pos_;
    //前フレームから移動しているなら歩き状態
    if(float1(0.01f) < dot(diff, diff)) {
        model->PlayAnimationNoSame("walk", true);
    }
    else {
        model->PlayAnimationNoSame("idle", true);
    }
    prev_pos_ = curr_pos;    //updateの末尾で、座標を保存しておく

    //---------------------------------------------------------------------------
    //HPが0で死亡
    //---------------------------------------------------------------------------
    if(auto status = owner->GetComponent<ComponentStatus>()) {
        if(status->GetHitPoints() <= 0) {
            ChangeState<StateDeath>();
        }
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
