//---------------------------------------------------------------------------
//!	@file	ComponentJump.cpp
//! @brief	ジャンプ機能コンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Component/ComponentJump.h>
#include <System/Component/ComponentRigidbody.h>
#include <System/Component/ComponentLiftable.h>

//---------------------------------------------------------------------------
//! @brief	初期化処理
//---------------------------------------------------------------------------
void ComponentJump::Init()
{
    __super::Init();
}

//---------------------------------------------------------------------------
//! @brief	更新処理
//---------------------------------------------------------------------------
void ComponentJump::Update()
{
    __super::Update();
    jump_frame_count_--;
    auto owner = GetOwner();
    //オーナーが持ち上げられ状態なら、後の処理は行わない。
    if(owner->GetComponent<ComponentLiftable>()->IsLifted())
        return;
    //スペースキー押下でジャンプ
    if(conditions_jump_() && (jump_frame_count_ < 0) && set_enable_ == false) {
        jump_frame_count_ = jump_frame_max_;
        owner->GetComponent<ComponentRigidbody>()->AddImpulse(float3(0.0f, jump_force_, 0.0f));
        is_jumping_ = true;
    }
    //ジャンプのカウントが0以下ならリターン(この後の処理を行わない)
    if(jump_frame_count_ < 0) {
        set_enable_ = false;
        is_jumping_ = false;
        return;
    }
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentJump::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"Jump")) {
            // GUI上でオーナーから自分(SampleObjectController)を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

//---------------------------------------------------------------------------
//! @brief	何フレームジャンプを行うかをセット
//---------------------------------------------------------------------------
void ComponentJump::SetJumpFrame(int value)
{
    jump_frame_max_ = value;
}

//---------------------------------------------------------------------------
//! @brief	ジャンプでどのくらい飛び上がるか
//---------------------------------------------------------------------------
void ComponentJump::SetJumpImpulse(float value)
{
    jump_force_ = value;
}

//---------------------------------------------------------------------------
//! @brief	有効無効をセット
//---------------------------------------------------------------------------
void ComponentJump::SetEnable(bool enable_flag)
{
    set_enable_ = enable_flag;
}

//---------------------------------------------------------------------------
//! @brief	ジャンプ中か否かを変換
//---------------------------------------------------------------------------
bool ComponentJump::IsJumping()
{
    return is_jumping_;
}

//---------------------------------------------------------------------------
//! @brief	ジャンプ条件を代入
//---------------------------------------------------------------------------
void ComponentJump::SetConditionsJump(std::function<bool()> condition)
{
    conditions_jump_ = condition;
}
CEREAL_REGISTER_TYPE(ComponentJump)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentJump)
