//---------------------------------------------------------------------------
//!	@file	StateBase.cpp
//! @brief	歩き&待機状態コンポーネント
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "StateIdleWalk.h"
#include <System/Component/ComponentSpringArm.h>
#include <System/Component/ComponentModel.h>

void StateIdleWalk::Init()
{
    __super::Init();
}

void StateIdleWalk::Update()
{
    __super::Update();

    auto owner = GetOwner();
    //アニメーション
    auto model = owner->GetComponent<ComponentModel>();
    if(CheckHitKey(key_up_) || CheckHitKey(key_down_) || CheckHitKey(key_left_) || CheckHitKey(key_right_)) {
        model->PlayAnimationNoSame("walk", true);
    }
    else {
        model->PlayAnimationNoSame("idle", true);
    }
}

StateIdleWalkPtr StateIdleWalk::SetKeys(int up, int down, int left, int right)
{
    // 移動キーの設定
    key_up_    = up;
    key_down_  = down;
    key_left_  = left;
    key_right_ = right;
    return std::dynamic_pointer_cast<StateIdleWalk>(shared_from_this());
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
