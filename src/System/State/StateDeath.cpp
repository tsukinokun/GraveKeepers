//---------------------------------------------------------------------------
//!	@file	StateDeath.cpp
//! @brief	死亡状態コンポーネント
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "StateDeath.h"
#include <System/Component/ComponentSpringArm.h>
#include <System/Component/ComponentModel.h>

void StateDeath::Init()
{
    __super::Init();
    auto owner = GetOwner();
    //死亡アニメーションの単発再生
    if(auto model = owner->GetComponent<ComponentModel>()) {
        model->PlayAnimationNoSame("death", false);
    }
}

void StateDeath::Update()
{
    __super::Update();

    auto owner = GetOwner();
}

void StateDeath::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"StateDeath")) {
            // GUI上でオーナーから自分(SampleObjectController)を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

CEREAL_REGISTER_TYPE(StateDeath)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, StateDeath)
