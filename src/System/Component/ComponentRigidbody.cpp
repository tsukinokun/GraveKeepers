#pragma once
//---------------------------------------------------------------------------
//!	@file	ComponentRigidbody.cpp
//! @brief	剛体コンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include <System/Component/ComponentRigidbody.h>

//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentRigidbody::Init()
{
    __super::Init();
}
//---------------------------------------------------------------------------
//! @brief	物理演算
//---------------------------------------------------------------------------
void ComponentRigidbody::PrePhysics()
{
    __super::PrePhysics();
    auto owner = GetOwner();
    owner->AddTranslate(velocity_);
    //減衰
    velocity_.x *= damping_ratio_;
    velocity_.y *= damping_ratio_;
    velocity_.z *= damping_ratio_;
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentRigidbody::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"Rigidbody")) {
            // GUI上でオーナーから自分(SampleObjectController)を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

//---------------------------------------------------------------------------
//! @brief	衝撃を与える
//---------------------------------------------------------------------------
void ComponentRigidbody::AddImpulse(const float3& impulse)
{
    velocity_ += impulse;
}

CEREAL_REGISTER_TYPE(ComponentRigidbody)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentRigidbody)
