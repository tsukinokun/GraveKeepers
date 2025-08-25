//---------------------------------------------------------------------------
//!	@file	Player.cpp
//! @brief	プレイヤー
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "Player.h"
#include <Game/Scene/Character/Zombie/Zombie.h>
#include <Game/Scene/Character/Werewolf/Werewolf.h>
#include <System/Component/ComponentObjectController.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentJump.h>
#include <System/Component/ComponentLift.h>
#include <System/Component/ComponentRigidbody.h>
#include <System/Component/ComponentLiftable.h>
#include <System/Component/ComponentStatus.h>
#include <System/SkillComponent/ComponentFireBall.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Player::Init()
{
    __super::Init();
    //auto chara		   = Scene::Object::Create<Zombie>();	 //テスト、プレイヤーでゾンビを作成、後々選択したものに変更する。
    auto chara         = Scene::Object::Create<Werewolf>();    //狼男を表示するためにゾンビをコメントアウトしています
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
    SetName(u8"プレイヤー");

    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Player::Update()
{
    __super::Update();
    //if(auto hp = GetComponent<ComponentStatus>())
    //{
    //	//死亡で
    //	if(hp->IsDead())
    //	{
    //		return;
    //	}
    //}
    ////下キーを押しているかつジャンプをしていないなら
    //if(!GetComponent<ComponentLiftable>()->IsLifted())
    //{
    //	if(CheckHitKey(KEY_INPUT_DOWN) && GetComponent<ComponentJump>()->IsJumping() == false)
    //	{
    //		//ジャンプをできない状態にする
    //		GetComponent<ComponentJump>()->SetEnable();
    //		//高さを半径にする
    //		neutralpos_ = SQUAT_TOP_POINT_;
    //	}
    //	//右のシフトキーを押しているかつジャンプをしていないなら
    //	else if(CheckHitKey(KEY_INPUT_RSHIFT) && GetComponent<ComponentJump>()->IsJumping() == false)
    //	{
    //		//ジャンプをできない状態にする
    //		GetComponent<ComponentJump>()->SetEnable();
    //		//高さを半径にする
    //		neutralpos_ = FACE_DOWN_TOP_POINT_;
    //		//しゃがんでいると返す
    //		is_face_down_ = true;
    //	}
    //	//上の状態でなかったら
    //	else
    //	{
    //		//高さを半径の3倍にする
    //		neutralpos_ = TOP_POINT_;
    //		//しゃがんでいないと返す
    //		is_face_down_ = false;
    //	}
    //}
    //else
    //{
    //	//高さを半径の3倍にする
    //	neutralpos_ = TOP_POINT_;
    //	//しゃがんでいないと返す
    //	is_face_down_ = false;
    //}
    ////コリジョンの高さの設定
    //GetComponent<ComponentCollisionCapsule>()->SetHeight(RADIUS_ + neutralpos_);
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void Player::Draw()
{
    //float3 sphire_pos = float3(GetTranslate() + float3(0.0f, neutralpos_, 0.0f));
    //DrawSphere3D(cast(sphire_pos), RADIUS_, 16, WHITE, WHITE, TRUE);
    //float3 cone_top	   = float3(sphire_pos.xyz);
    //float3 rot		   = GetRotationAxisXYZ();
    //float3 cone_bottom = float3(sphire_pos.x + (-5 * sinf(D2R(rot.y))), sphire_pos.y, sphire_pos.z + (-5 * cosf(D2R(rot.y))));
    //DrawCone3D(cast(cone_bottom), cast(cone_top), RADIUS_, 16, WHITE, WHITE, TRUE);

    ////うつ伏せの状態ではなかったら
    //if(!is_face_down_)
    //{
    //	//身体を表示する
    //	DrawCone3D(cast(sphire_pos), cast(GetTranslate()), RADIUS_, 16, WHITE, WHITE, TRUE);
    //}
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
