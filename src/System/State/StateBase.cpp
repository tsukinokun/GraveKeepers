//---------------------------------------------------------------------------
//!	@file	StateBase.cpp
//! @brief	状態のベースコンポーネント
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "StateBase.h"
#include <System/Component/ComponentModel.h>

void StateBase::Init()
{
    __super::Init();
}

void StateBase::Update()
{
    __super::Update();
}

void StateBase::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"StateBase")) {
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

CEREAL_REGISTER_TYPE(StateBase)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, StateBase)
