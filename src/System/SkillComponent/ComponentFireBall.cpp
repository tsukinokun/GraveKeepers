//---------------------------------------------------------------------------
//!	@file	ComponentFireBall.cpp
//! @brief	ファイアボールのコンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentFireBall.h"

//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentFireBall::Init()
{    // 初期化処理
    __super::Init();
}

//---------------------------------------------------------------------------
//! @brief	UI描画
//---------------------------------------------------------------------------
void ComponentFireBall::LateDraw()
{
    __super::LateDraw();
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentFireBall::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"ファイアボールコンポーネント")) {
            // GUI上でオーナーから自分(SampleObjectController)を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

CEREAL_REGISTER_TYPE(ComponentFireBall)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentFireBall)
