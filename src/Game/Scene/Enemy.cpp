//---------------------------------------------------------------------------
//!	@file	Enemy.cpp
//! @brief	エネミー
//---------------------------------------------------------------------------
#include "Enemy.h"
#include <Game/Scene/Character/Zombie/Zombie.h>
#include <System/Component/ComponentObjectController.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentJump.h>
#include <System/Component/ComponentRigidbody.h>
#include <System/Component/ComponentLift.h>
#include <System/Component/ComponentLiftable.h>
#include <System/Component/ComponentStatus.h>
#include <System/Component/ComponentAI.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Enemy::Init()
{
    __super::Init();
    auto chara = Scene::Object::Create<Zombie>();    //テスト、プレイヤーでゾンビを作成、後々選択したものに変更する。
    chara->AddComponent<ComponentAI>();
    if(auto jump_comp = chara->GetComponent<ComponentJump>()) {
        jump_comp->SetConditionsJump([]() { return false; });
    }

    if(auto lift_comp = chara->GetComponent<ComponentLift>()) {
        lift_comp->SetConditionsForLifting(
            //ラムダ式を代入
            []() { return true; });
        lift_comp->SetConditionsForThrow(    //ラムダ式を代入
            [this]() {
                if(auto controll_lock = controll_character_.lock()) {
                    if(auto ai = controll_lock->GetComponent<ComponentAI>()) {
                        return ai->ThrowSignal();
                    }
                }
            });
    }
    controll_character_ = chara;

    SetName(u8"エネミー");

    return true;
}
