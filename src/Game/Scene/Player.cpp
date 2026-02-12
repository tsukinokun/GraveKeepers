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
#include <Game/Scene/Character/CharacterFactory.h>
#include <Game/System/GameRepository.h>
#include <Game/Scene/UIObject/UIImage.h>
#include "../../src/Game/system/HlslppUseful.h"
#include <Game/System/ImageBuffer.h>
#include < Game/System/SoundBuffer.h>
#include <Game/Scene/SkillObject/SkillFactory.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Player::Init()
{
    __super::Init();
    //----------------------------------------------
    // 選択したキャラクター生成
    //----------------------------------------------
    auto chara = CharacterFactory::Instance().CreateCharacter(GameRepository::Instance().GetSelectedCharacterName());
    //chara->SetTranslate(float3(20.0f, 1.0f, 20.0f));
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
    if(auto lift_comp = chara->GetComponent<ComponentLift>()) {
        lift_comp->SetConditionsForLifting(
            //ラムダ式を代入
            []() {
                if(IsKeyOn(KEY_INPUT_RETURN)) {
                    return true;
                }
                return false;
            });
        lift_comp->SetConditionsForThrow(    //ラムダ式を代入
            []() {
                if(IsKeyOn(KEY_INPUT_RETURN)) {
                    return true;
                }
                return false;
            });
    }

    //----------------------------------------------
    // 選択したスキルを追加
    //----------------------------------------------
    std::string skill_name = GameRepository::Instance().GetSelectedSkillName();
    auto        skill      = SkillFactory::Instance().CreateSkill(skill_name);
    if(skill) {
        //skillで取得した名前からスキルコンポーネントを追加
        skill_name = GameRepository::Instance().GetSelectedSkillName();
    }

    std::shared_ptr<ComponentSkill> skill_component;    //スキルコンポーネントを操作するためのポインタ
    if(skill_name == "FireBall") {
        skill_component = chara->AddComponent<ComponentFireBall>();
    }
    else if(skill_name == "Dash") {
        skill_component = chara->AddComponent<ComponentDash>();
    }
    else if(skill_name == "Poison") {
        skill_component = chara->AddComponent<ComponentPoison>();
    }
    else if(skill_name == "ComboAttack") {
        skill_component = chara->AddComponent<ComponentComboAttack>();
    }
    //共通のスキル発動条件設定
    skill_component->SetConditionsForUseSkill(
        //ラムダ式を代入、Pキーを押すとスキル発動と割り当てる。
        []() {
            if(IsKeyOn(KEY_INPUT_P)) {
                return true;
            }
            return false;
        });
    {
        //画像を読み込む
        auto test_image = Scene::Object::Create<UIImage>(u8"プレイヤー識別画像");
        test_image->SetImage(ImageBuffer::GetImageHandle("star"));    // 画像ハンドルを設定

        auto update_proc = [test_image, this, chara]() {
            if(auto camera = Scene::GetCurrentCamera().lock()) {
                //画像の位置を設定
                test_image->SetTranslate(
                    float3(WorldPositionToScreenPosition(chara->GetTranslate() + float3(0.0f, 20.0f, 0.0f)), 0.0f));    //ゲージの位置を設定

                //画像の大きさを設定
                test_image->SetScaleAxisXYZ(0.1f);
            }
            else {
                //表示しない
                test_image->SetScaleAxisXYZ(0.0f);
            }
        };
        test_image->SetProc("update", update_proc);
    }

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

//---------------------------------------------------------------------------
//! @brief コントロールしているキャラクターを取得
//---------------------------------------------------------------------------
std::weak_ptr<Character> Player::GetControllCharacter() const
{
    return controll_character_;
}
