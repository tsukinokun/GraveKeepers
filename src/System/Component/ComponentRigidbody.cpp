#pragma once
//---------------------------------------------------------------------------
//!	@file	ComponentRigidbody.cpp
//! @brief	剛体コンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include <System/Component/ComponentRigidbody.h>
float3 ComponentRigidbody::gravity_ = float3(0.0f, -9.8f, 0.0f);

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
    //時間の更新
    current_time_    = std::chrono::high_resolution_clock::now();
    auto delta_time_ = std::chrono::duration<float>(current_time_ - prev_time_).count();
    prev_time_       = current_time_;
    //オーナーを取得
    auto owner = GetOwner();
    //加速度分を速度に加算
    velocity_ += (acceleration_ / mass_) * delta_time_;
    //重力分を速度に加算
    velocity_ += gravity_ * mass_ * delta_time_;
    //速度を加算
    owner->AddTranslate(velocity_ * delta_time_);
    //減衰
    velocity_ *= (1.0f - drag_ * delta_time_);
    //加速度をクリア
    acceleration_ = float3(0.0f, 0.0f, 0.0f);
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
//! @brief	力を与える
//---------------------------------------------------------------------------
void ComponentRigidbody::AddForce(const float3& force)
{
    acceleration_ += force;
}

//---------------------------------------------------------------------------
//! @brief	衝撃を与える
//---------------------------------------------------------------------------
void ComponentRigidbody::AddImpulse(const float3& impulse)
{
    velocity_ += impulse;
}

//---------------------------------------------------------------------------
//! @brief  重力を使用するかを変更
//---------------------------------------------------------------------------
void ComponentRigidbody::SetUseGravity(bool use_gravity_flag)
{
    use_gravity_ = use_gravity_flag;
}

//---------------------------------------------------------------------------
//! @brief  速度を取得
//---------------------------------------------------------------------------
float3 ComponentRigidbody::GetVelocity() const
{
    return velocity_;
}

//---------------------------------------------------------------------------
//! @brief  質量を取得
//---------------------------------------------------------------------------
float ComponentRigidbody::GetMass() const
{
    return mass_;
}

//---------------------------------------------------------------------------
//! @brief  反発係数を設定する
//---------------------------------------------------------------------------
void ComponentRigidbody::SetRestitution(float restitution)
{
    restitution_ = restitution;
}
CEREAL_REGISTER_TYPE(ComponentRigidbody)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentRigidbody)
