//---------------------------------------------------------------------------
//!	@file	ComponentSkill.cpp
//! @brief	文字列コンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentSkill.h"
#include <System/Component/ComponentStatus.h>

//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentSkill::Init()
{    // 初期化処理
    __super::Init();
}

//---------------------------------------------------------------------------
//! @brief	更新処理関数
//---------------------------------------------------------------------------
void ComponentSkill::Update()
{    // 初期化処理
    __super::Update();
    //スキル使用条件を満たしたら
    if(is_use_skill_() && CanUseSkill()) {
        UseSkill();    // スキルを使用する
    }
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentSkill::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"スキルコンポーネント")) {
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

//---------------------------------------------------------------------------
//! @brief	スキルを発動条件を設定する
//---------------------------------------------------------------------------
std::shared_ptr<ComponentSkill> ComponentSkill::SetConditionsForUseSkill(const std::function<bool()>& is_use_skill)
{
    is_use_skill_ = is_use_skill;
    return dynamic_pointer_cast<ComponentSkill>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief	スキルを発動する
//---------------------------------------------------------------------------
std::shared_ptr<ComponentSkill> ComponentSkill::UseSkill()
{
    auto owner = GetOwner();
    //ステータスコンポーネントを取得
    if(auto status_comp = owner->GetComponent<ComponentStatus>()) {
        //MPを消費する
        status_comp->DepleteMagicPoint();
    }
    return dynamic_pointer_cast<ComponentSkill>(shared_from_this());
}

//--------------------------------------------------------------------
//! @brief スキルを発動できるかを返す関数
//--------------------------------------------------------------------
bool ComponentSkill::CanUseSkill() const
{
    // オーナーを取得
    auto owner = GetOwner();
    //--------------------------------------------------------------------
    //ステータスコンポーネントを取得
    //--------------------------------------------------------------------
    if(auto status_comp = owner->GetComponent<ComponentStatus>()) {
        //--------------------------------------------------------------------
        //MPが満タンでなければスキルを使用できない
        //--------------------------------------------------------------------
        if(!status_comp->IsMagicPointFull())
            return false;
        //--------------------------------------------------------------------
        // 死亡している場合はスキルを使用できない
        //--------------------------------------------------------------------
        if(status_comp->IsDead())
            return false;
    }
    return true;    // スキルを発動できる
}

CEREAL_REGISTER_TYPE(ComponentSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentSkill)
