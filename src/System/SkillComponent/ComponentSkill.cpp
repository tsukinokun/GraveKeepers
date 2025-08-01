//---------------------------------------------------------------------------
//!	@file	ComponentSkill.cpp
//! @brief	文字列コンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentSkill.h"

//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentSkill::Init()
{    // 初期化処理
    __super::Init();
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

CEREAL_REGISTER_TYPE(ComponentSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentSkill)
