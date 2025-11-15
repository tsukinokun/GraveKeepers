//---------------------------------------------------------------------------
//!	@file	Player.cpp
//! @brief	プレイヤー
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "Player.h"
#include <Game/Scene/Character/Zombie/Zombie.h>
#include <Game/Scene/Character/Werewolf/Werewolf.h>
#include <Game/Scene/Character/Pumpking/Pumpking.h>
#include <Game/Scene/Character/Witch/Witch.h>
#include <System/Component/ComponentObjectController.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentJump.h>
#include <System/Component/ComponentLift.h>
#include <System/Component/ComponentRigidbody.h>
#include <System/Component/ComponentLiftable.h>
#include <System/Component/ComponentStatus.h>
#include <System/SkillComponent/ComponentFireBall.h>
#include <System/SkillComponent/ComponentDash.h>
#include <System/SkillComponent/ComponentPoison.h>
#include <System/SkillComponent/ComponentComboAttack.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Player::Init()
{
    __super::Init();
    auto chara = Scene::Object::Create<Zombie>();    //テスト、プレイヤーでゾンビを作成、後々選択したものに変更する。
    chara->SetTranslate(float3(20.0f, 1.0f, 20.0f));
    auto controll_comp = chara->AddComponent<ComponentObjectController>();
    controll_comp->SetMoveSpeed(chara->GetComponent<ComponentStatus>()->GetSpeed());
    controll_comp->SetRotateSpeed(20.0f);
    controll_character_ = chara;
    if(auto jump_comp = chara->GetComponent<ComponentJump>()) {
        jump_comp->SetConditionsJump([]() {
            if(IsKeyOn(KEY_INPUT_SPACE))
                return true;
            return false;
        });
    }
    //col_comp->SetCollisionGroup(ComponentCollision::CollisionGroup::PLAYER);	// 所属するグループを「PLAYER」とします
    //auto lift_comp = AddComponent<ComponentLift>();								//持ち上げコンポーネント
    if(auto lift_comp = chara->GetComponent<ComponentLift>()) {
        lift_comp->SetConditionsForLifting(
            //ラムダ式を代入
            []() {
                if(IsKeyOn(KEY_INPUT_Z)) {
                    return true;
                }
                return false;
            });
        lift_comp->SetConditionsForThrow(    //ラムダ式を代入
            []() {
                if(IsKeyOn(KEY_INPUT_Z)) {
                    return true;
                }
                return false;
            });
    }
    //----------------------------------------------
    //スキルコンポーネント(仮でファイアーボールを付ける)の設定
    //----------------------------------------------
    auto fireball_comp = chara->AddComponent<ComponentFireBall>();
    fireball_comp->SetConditionsForUseSkill(
        //ラムダ式を代入、xキーを押すとスキル仕様と割り当てる。
        []() {
            if(IsKeyOn(KEY_INPUT_X)) {
                return true;
            }
            return false;
        });

    //----------------------------------------------
    //スキルコンポーネント(仮で突進)の設定
    //----------------------------------------------
    auto dash_comp = chara->AddComponent<ComponentDash>();
    dash_comp->SetConditionsForUseSkill(
        //ラムダ式を代入、Bキーを押すとスキル仕様と割り当てる。
        []() {
            if(IsKeyOn(KEY_INPUT_B)) {
                return true;
            }
            return false;
        });
    //----------------------------------------------
    //スキルコンポーネント(仮で毒設置アニメーション)の設定
    //----------------------------------------------
    auto poison_comp = chara->AddComponent<ComponentPoison>();
    poison_comp->SetConditionsForUseSkill(
        //ラムダ式を代入、cキーを押すとスキル仕様と割り当てる。
        []() {
            if(IsKeyOn(KEY_INPUT_C)) {
                return true;
            }
            return false;
        });
    //スキルコンポーネント(仮で連撃を付ける)の設定
    //----------------------------------------------
    auto combo_attack_comp = chara->AddComponent<ComponentComboAttack>();
    combo_attack_comp->SetConditionsForUseSkill(
        //ラムダ式を代入、vキーを押すとスキル仕様と割り当てる。
        []() {
            if(IsKeyOn(KEY_INPUT_V)) {
                return true;
            }
            return false;
        });
    SetName(u8"プレイヤー");

    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Player::Update()
{
    __super::Update();
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void Player::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void Player::Exit()
{
    __super::Exit();
}

//!GUI表示
void Player::GUI()
{
    __super::GUI();
}

void Player::OnHit(const ComponentCollision::HitInfo& hit_info)
{
    __super::OnHit(hit_info);
    auto hit_owner = hit_info.hit_collision_->GetOwner();
    if(auto hit_liftable = hit_owner->GetComponent<ComponentLiftable>()) {
        //持ち上げられ中(空中)でなければ
        if(!hit_liftable->IsLifted()) {
            return;    //早期リターン
        }
        //剛体を取得
        if(auto hit_rb = hit_owner->GetComponent<ComponentRigidbody>()) {
            //触ったオブジェクトの速度が少しでもあれば
            if(length(hit_rb->GetVelocity()) > float1(1.0f)) {
                GetComponent<ComponentRigidbody>()->AddImpulse(hit_rb->GetVelocity());
                int damage = static_cast<int>(hit_rb->GetMass());       //ダメージは当たったオブジェクトの質量に比例
                GetComponent<ComponentStatus>()->TakeDamage(damage);    //ダメージを受ける
            }
        }
    }
}
