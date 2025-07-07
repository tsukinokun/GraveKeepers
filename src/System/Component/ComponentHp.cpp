//---------------------------------------------------------------------------
//!	@file	ComponentHp.cpp
//! @brief	HP機能コンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Component/ComponentHp.h>

//---------------------------------------------------------------------------
//! @brief	初期化処理
//---------------------------------------------------------------------------
void ComponentHp::Init()
{
    __super::Init();
}

//---------------------------------------------------------------------------
//! @brief	更新処理
//---------------------------------------------------------------------------
void ComponentHp::Update()
{
    //時間の更新
    current_time_    = std::chrono::high_resolution_clock::now();
    auto delta_time_ = std::chrono::duration<float>(current_time_ - prev_time_).count();
    prev_time_       = current_time_;

    invincibility_timer_ -= delta_time_;    //タイマーからデルタタイムを引く
    __super::Update();
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentHp::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"Hp")) {
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
int ComponentHp::GetHitPoints() const
{
    return hp_;
}

//---------------------------------------------------------------------------
//! @brief	HPのセッタ
//---------------------------------------------------------------------------

void ComponentHp::SetHitPoints(int hp)
{
    hp_ = hp;
}

//---------------------------------------------------------------------------
//! @brief	ダメージを受ける関数
//---------------------------------------------------------------------------
void ComponentHp::TakeDamage(int damage)
{
    //タイマーが0.0fよりも上なら無敵時間中なので早期リターンを行う。
    if(invincibility_timer_ > 0.0f) {
        return;
    }
    hp_                  -= damage;                 //ダメージを受けて
    invincibility_timer_  = INVINCIBILITY_TIME_;    //無敵時間分を代入
}
CEREAL_REGISTER_TYPE(ComponentHp)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentHp)
