//---------------------------------------------------------------------------
//!	@file	ComponentStatus.cpp
//! @brief	ステータス機能コンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Component/ComponentStatus.h>

//---------------------------------------------------------------------------
//! @brief	初期化処理
//---------------------------------------------------------------------------
void ComponentStatus::Init()
{
    __super::Init();
}

//---------------------------------------------------------------------------
//! @brief	更新処理
//---------------------------------------------------------------------------
void ComponentStatus::Update()
{
    //時間の更新
    current_time_   = std::chrono::high_resolution_clock::now();
    auto delta_time = std::chrono::duration<float>(current_time_ - prev_time_).count();
    prev_time_      = current_time_;

    invincibility_timer_ -= delta_time;    //タイマーからデルタタイムを引く
    __super::Update();
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentStatus::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"ステータス")) {
            // GUI上でオーナーから自分(SampleObjectController)を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

//---------------------------------------------------------------------------
//! @brief	HPのゲッタ
//---------------------------------------------------------------------------
int ComponentStatus::GetHitPoints() const
{
    return hp_;
}

//---------------------------------------------------------------------------
//! @brief	HPのセッタ
//---------------------------------------------------------------------------

std::shared_ptr<ComponentStatus> ComponentStatus::SetHitPoints(int hp)
{
    hp_ = hp;
    return dynamic_pointer_cast<ComponentStatus>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief	ダメージを受ける関数
//---------------------------------------------------------------------------
std::shared_ptr<ComponentStatus> ComponentStatus::TakeDamage(int damage)
{
    //HPが0以下なら当然これ以上ダメージを受けない
    if(IsDead()) {
        return dynamic_pointer_cast<ComponentStatus>(shared_from_this());
    }
    //タイマーが0.0fよりも上なら無敵時間中なので早期リターンを行う。
    if(invincibility_timer_ > 0.0f) {
        return dynamic_pointer_cast<ComponentStatus>(shared_from_this());
    }
    hp_                  -= damage;                 //ダメージを受けて
    hp_                   = std::max(0, hp_);       //0.0fより小さくならない
    invincibility_timer_  = INVINCIBILITY_TIME_;    //無敵時間分を代入
    return dynamic_pointer_cast<ComponentStatus>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief	HPが0であるかを返す関数
//---------------------------------------------------------------------------
bool ComponentStatus::IsDead() const
{
    if(hp_ <= 0) {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
//! @brief	移動スピードのセッタ
//---------------------------------------------------------------------------
std::shared_ptr<ComponentStatus> ComponentStatus::SetSpeed(float move_speed)
{
    move_speed_ = move_speed;
    return dynamic_pointer_cast<ComponentStatus>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief	移動スピードのゲッタ
//---------------------------------------------------------------------------
float ComponentStatus::GetSpeed() const
{
    return move_speed_;
}
CEREAL_REGISTER_TYPE(ComponentStatus)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentStatus)
