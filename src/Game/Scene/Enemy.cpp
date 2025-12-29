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
#include <Game/Scene/Character/CharacterFactory.h>
#include <Game/System/GameRepository.h>
#include <algorithm>
#include <random>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Enemy::Init()
{
    __super::Init();
    // 生成するキャラ名を決定（desired_character_name_ がセットされていればそれを優先）
    std::string create_name;

    if(!desired_character_name_.empty()) {
        create_name = desired_character_name_;
    }
    else {
        // プレイヤーが選んだキャラを除外してランダム選択
        auto names           = CharacterFactory::Instance().GetRegisteredCharacterNames();
        auto player_selected = GameRepository::Instance().GetSelectedCharacterName();
        names.erase(std::remove(names.begin(), names.end(), player_selected), names.end());

        if(!names.empty()) {
            std::random_device rd;
            std::mt19937       gen(rd());
            std::shuffle(names.begin(), names.end(), gen);
            create_name = names.front();
        }
        else {
            // 候補がない場合はフォールバック
            create_name = "Zombie";
        }
    }

    // キャラクター生成（Factory を使う。失敗時は Zombie）
    auto chara = CharacterFactory::Instance().CreateCharacter(create_name);
    if(!chara) {
        chara = Scene::Object::Create<Zombie>();
    }

    chara->AddComponent<ComponentAI>();
    //chara->RemoveComponent<ComponentLift>();
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
                return false;
            });
    }
    controll_character_ = chara;

    SetName(u8"エネミー");

    return true;
}

//---------------------------------------------------------------------------
//! @brief コントロールしているキャラクターを取得
//---------------------------------------------------------------------------
std::weak_ptr<Character> Enemy::GetControllCharacter() const
{
    return controll_character_;
}
