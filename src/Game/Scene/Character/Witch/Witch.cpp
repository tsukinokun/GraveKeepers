//---------------------------------------------------------------------------
//!	@file	Witch.cpp
//! @brief	インゲームシーンの魔女
//! @author 上田朋輝
//---------------------------------------------------------------------------
#include "Witch.h"
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
bool Witch::Init()
{
    __super::Init();
    if(auto model_comp = model_component_.lock()) {
        model_comp->Load("data/PoyPoy/Model/Character/Witch/Witch.mv1");
        model_comp->SetAnimation({
            { "idle",                 "data/PoyPoy/Model/Character/Witch/Anims/Idle.mv1", 0, 1.0f},
            { "walk",              "data/PoyPoy/Model/Character/Witch/Anims/Walking.mv1", 0, 1.0f},
            {"death", "data/PoyPoy/Model/Character/Witch/Anims/Death From The Front.mv1", 0, 1.0f},
            { "jump",                 "data/PoyPoy/Model/Character/Witch/Anims/Jump.mv1", 0, 0.7f},
        });
    }
    if(auto status_comp = status_component_.lock()) {
        status_comp->SetHitPoints(HP_MAX_);
        status_comp->SetSpeed(MOVE_SPEED_);
        status_comp->SetAttackPoints(ATTACK_POWER_);
    }
    SetName(u8"Witch");
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Witch::Update()
{
    __super::Update();
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void Witch::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void Witch::Exit()
{
    __super::Exit();
}

//!GUI表示
void Witch::GUI()
{
    __super::GUI();
}
