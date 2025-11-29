//---------------------------------------------------------------------------
//!	@file	Pumpking.cpp
//! @brief	インゲームシーンの狼男
//! @author
//---------------------------------------------------------------------------
#include "Pumpking.h"
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
bool Pumpking::Init()
{
    __super::Init();
    if(auto model_comp = model_component_.lock()) {
        model_comp->Load("data/PoyPoy/Model/Character/Pumpking/Pumpking.mv1");

        model_comp->SetAnimation({
            {          "idle",           "data/PoyPoy/Model/Character/Pumpking/Anims/Idle.mv1", 1, 1.0f}, //待機
            {          "walk",           "data/PoyPoy/Model/Character/Pumpking/Anims/Walk.mv1", 1, 1.0f}, //歩き
            {           "run",            "data/PoyPoy/Model/Character/Pumpking/Anims/Run.mv1", 1, 1.0f}, //走り
            {         "death", "data/PoyPoy/Model/Character/Pumpking/Anims/DeathMotionLay.mv1", 1, 1.0f}, //死亡
            {          "jump",           "data/PoyPoy/Model/Character/Pumpking/Anims/Jump.mv1", 1, 1.0f}, //ジャンプ
            {"crouching_idle",  "data/PoyPoy/Model/Character/Pumpking/Anims/CrouchingIdle.mv1", 1, 1.0f}, //しゃがみ待機
            {"crouching_walk",  "data/PoyPoy/Model/Character/Pumpking/Anims/CrouchingWalk.mv1", 1, 1.0f}, //しゃがみ歩き
            { "crouching_run",   "data/PoyPoy/Model/Character/Pumpking/Anims/CrouchingRun.mv1", 1, 1.0f}, //しゃがみ走り
            {     "low_crawl",       "data/PoyPoy/Model/Character/Pumpking/Anims/LowCrawl.mv1", 1, 1.0f}, //這いずり移動
        });
    }
    if(auto status_comp = status_component_.lock()) {
        status_comp->SetHitPoints(HP_MAX_);
        status_comp->SetSpeed(MOVE_SPEED_);
    }
    //SetName(u8"狼男");
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Pumpking::Update()
{
    __super::Update();
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void Pumpking::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void Pumpking::Exit()
{
    __super::Exit();
}

//!GUI表示
void Pumpking::GUI()
{
    __super::GUI();
}
