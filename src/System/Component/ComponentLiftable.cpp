//---------------------------------------------------------------------------
//!	@file	ComponentLiftable.cpp
//! @brief	持ち上げられ機能コンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Component/ComponentLiftable.h>
#include <System/Component/ComponentHitInfo.h>

//---------------------------------------------------------------------------
//! @brief	初期化
//---------------------------------------------------------------------------
void ComponentLiftable::Init()
{
    __super::Init();
    //---------------------------------------------------------------------------
    // ラムダ式にヒット時のコールバックを
    //---------------------------------------------------------------------------
    OnHitComponentFunc = [this](const HitInfo& hit_info) {
        auto hit_owner = hit_info.hit_collision_->GetOwner();
        //地面に当たったら
        if(hit_owner->GetNameDefault() == u8"Field") {
            can_be_lifted_ = true;    //もう一度持ち上げ可能にする
        }
    };
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentLiftable::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"Lifted")) {
            // GUI上でオーナーから自分(SampleObjectController)を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

//----------------------------------------------------------------
//! @brief 持ち上げられているかの状態を変更
//----------------------------------------------------------------
void ComponentLiftable::SetLiftedFlag(bool lifted_flag)
{
    is_lifted_ = lifted_flag;
}

//----------------------------------------------------------------
//! @brief	持ち上げられているか否かを返す関数
//----------------------------------------------------------------
bool ComponentLiftable::IsLifted()
{
    return is_lifted_;
}

//----------------------------------------------------------------
//! @brief 持ち上げたキャラクターの弱参照を設定する関数
//----------------------------------------------------------------
void ComponentLiftable::SetLiftCharacter(const std::weak_ptr<Character>& character)
{
    lift_character_ = character;
}

//--------------------------------------------------------------------
//! @brief このオブジェクトが持ち上げ可能かを返す関数
//--------------------------------------------------------------------
bool ComponentLiftable::CanBeLifted() const
{
    return can_be_lifted_;
}

//--------------------------------------------------------------------
//! @brief 持ち上げ不可に変更する関数
//--------------------------------------------------------------------
void ComponentLiftable::SetCannotBeLifted()
{
    can_be_lifted_ = false;
}

//----------------------------------------------------------------
//! @brief 最後に持ち上げたキャラクターの参照を取得する関数
//----------------------------------------------------------------
std::shared_ptr<Character> ComponentLiftable::GetLiftCharacter()
{
    return lift_character_.lock();
}
CEREAL_REGISTER_TYPE(ComponentLiftable)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentLiftable)
