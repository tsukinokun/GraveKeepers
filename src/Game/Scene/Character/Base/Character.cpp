//---------------------------------------------------------------------------
//!	@file	Character.cpp
//! @brief	キャラクターのベースクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "Character.h"
#include <System/Component/ComponentObjectController.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentJump.h>
#include <System/Component/ComponentLift.h>
#include <System/Component/ComponentRigidbody.h>
#include <System/Component/ComponentLiftable.h>
#include <System/Component/ComponentStatus.h>
#include <System/State/StateIdleWalk.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Character::Init()
{
    __super::Init();
    rigidbody_component_ = AddComponent<ComponentRigidbody>();    //剛体コンポーネントを追加
    liftable_component_  = AddComponent<ComponentLiftable>();     //持ち上げられ機能コンポーネント
    auto hp_comp         = AddComponent<ComponentStatus>();       //HP機能コンポーネント
    status_component_    = hp_comp;
    SetTranslate({0, 2, 0});
    auto col_comp = AddComponent<ComponentCollisionCapsule>();
    col_comp->SetRadius(RADIUS_);                  // 球コリジョンの半径を2.0 にする
    col_comp->SetHeight(RADIUS_ + neutralpos_);    // 球コリジョンの高さを半径の４倍 にする
    collision_component_ = col_comp;
    auto jump_comp       = AddComponent<ComponentJump>();
    jump_component_      = jump_comp;
    auto model           = AddComponent<ComponentModel>();    //剛体コンポーネントを追加
    model->SetRotationAxisXYZ(float3(0.0f, 180.0f, 0.0f));    //モデルの回転を設定
    model_component_ = model;
    lift_component_  = AddComponent<ComponentLift>();    //持ち上げコンポーネント
    //---------------------------------------------------------------------------------
    //状態コンポーネントをつける
    //---------------------------------------------------------------------------------
    AddComponent<StateIdleWalk>();
    SetName(u8"Character");

    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Character::Update()
{
    __super::Update();
    if(auto hp = GetComponent<ComponentStatus>()) {
        //死亡で
        if(hp->IsDead()) {
            return;
        }
    }
    //下キーを押しているかつジャンプをしていないなら
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
void Character::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void Character::Exit()
{
    __super::Exit();
}

//!GUI表示
void Character::GUI()
{
    __super::GUI();
}

void Character::OnHit(const ComponentCollision::HitInfo& hit_info)
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
