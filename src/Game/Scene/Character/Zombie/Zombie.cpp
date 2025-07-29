//---------------------------------------------------------------------------
//!	@file	Zombie.cpp
//! @brief	インゲームシーンのゾンビ
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "Zombie.h"
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
bool Zombie::Init()
{
    __super::Init();
    if(auto model_comp = model_component_.lock()) {
        model_comp->Load("data/PoyPoy/Model/Character/Zombie/Zombie.mv1");
    }
    if(auto status_comp = status_component_.lock()) {
        status_comp->SetHitPoints(HP_MAX_);
        status_comp->SetSpeed(MOVE_SPEED_);
    }
    SetName(u8"ゾンビ");
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Zombie::Update()
{
    __super::Update();
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void Zombie::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void Zombie::Exit()
{
    __super::Exit();
}

//!GUI表示
void Zombie::GUI()
{
    __super::GUI();
}
