//---------------------------------------------------------------------------
//!	@file	Werewolf.cpp
//! @brief	インゲームシーンのゾンビ
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "Werewolf.h"
#include <System/Component/ComponentObjectController.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentJump.h>
#include <System/Component/ComponentLift.h>
#include <System/Component/ComponentRigidbody.h>
#include <System/Component/ComponentLiftable.h>
#include <System/Component/ComponentStatus.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Werewolf::Init()
{
    __super::Init();
    if(auto model_comp = model_component_.lock()) {
        model_comp->Load("data/PoyPoy/Model/Character/Werewolf/Werewolf.mv1");
    }
    if(auto status_comp = status_component_.lock()) {
        status_comp->SetHitPoints(HP_MAX_);
        status_comp->SetSpeed(MOVE_SPEED_);
    }
    SetName(u8"狼男");
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Werewolf::Update()
{
    __super::Update();
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void Werewolf::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void Werewolf::Exit()
{
    __super::Exit();
}

//!GUI表示
void Werewolf::GUI()
{
    __super::GUI();
}
