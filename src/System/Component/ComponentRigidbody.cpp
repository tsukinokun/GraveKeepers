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

void ComponentRigidbody::PrePhysics()
{
    __super::PrePhysics();

    current_time_   = std::chrono::high_resolution_clock::now();
    auto delta_time = std::chrono::duration<float>(current_time_ - prev_time_).count();
    prev_time_      = current_time_;

    auto owner = GetOwner();

    // 力から加速度を速度に反映
    velocity_ += acceleration_ * delta_time;

    // 重力
    if(use_gravity_)
        velocity_ += gravity_ * delta_time;

    // 位置更新
    owner->AddTranslate(velocity_ * delta_time);

    // 減衰（指数減衰で安定）
    velocity_ *= std::exp(-drag_ * delta_time);

    // 加速度クリア
    acceleration_ = float3(0.0f);
}

void ComponentRigidbody::GUI()
{
    __super::GUI();

    if(ImGui::TreeNode(u8"Rigidbody")) {
        if(ImGui::Button(u8"削除"))
            GetOwner()->RemoveComponent(shared_from_this());

        ImGui::TreePop();
    }
}

void ComponentRigidbody::AddForce(const float3& force)
{
    // F = m * a → a = F / m
    acceleration_ += force / mass_;
}

void ComponentRigidbody::AddImpulse(const float3& impulse)
{
    // インパルスは速度変化量として直接加算
    velocity_ += impulse;
}

void ComponentRigidbody::SetUseGravity(bool use_gravity_flag)
{
    use_gravity_ = use_gravity_flag;
}

void ComponentRigidbody::SetVelocity(float3 velocity)
{
    velocity_ = velocity;
}

float3 ComponentRigidbody::GetVelocity() const
{
    return velocity_;
}

float ComponentRigidbody::GetMass() const
{
    return mass_;
}

void ComponentRigidbody::SetRestitution(float restitution)
{
    restitution_ = restitution;
}

float ComponentRigidbody::GetRestitution() const
{
    return restitution_;
}

void ComponentRigidbody::SetMass(float mass)
{
    mass_ = mass;
}
CEREAL_REGISTER_TYPE(ComponentRigidbody)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentRigidbody)
