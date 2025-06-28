//---------------------------------------------------------------------------
//!	@file	Enemy.cpp
//! @brief	エネミー
//---------------------------------------------------------------------------
#include "Enemy.h"
#include <System/Component/ComponentObjectController.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentJump.h>
#include <System/Component/ComponentRigidbody.h>
#include <System/Component/ComponentLift.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Enemy::Init()
{
    __super::Init();
    AddComponent<ComponentRigidbody>();
    SetTranslate({GetRand(PUT_RADIUS_MAX_) - PUT_RADIUS_MAX_ / 2, 2, GetRand(PUT_RADIUS_MAX_) - PUT_RADIUS_MAX_ / 2});
    auto col_comp = AddComponent<ComponentCollisionCapsule>();
    col_comp->UseGravity();
    col_comp->SetRadius(RADIUS_);                   // 球コリジョンの半径を2.0 にする
    col_comp->SetHeight(RADIUS_ + neutral_pos_);    // 球コリジョンの高さを半径の４倍 にする
    auto jump_comp = AddComponent<ComponentJump>();

    squat_timer_     = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;    //AIができたら消してください
    jump_timer_      = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;    //AIができたら消してください
    face_down_timer_ = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;    //AIができたら消してください
    lift_timer_      = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;    //AIができたら消してください
    throw_timer_     = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;    //AIができたら消してください

    jump_comp->SetConditionsJump([this]() {
        if(jump_timer_ < 0) {
            return true;
        }
        return false;
    });

    auto lift_comp = AddComponent<ComponentLift>();    //持ち上げコンポーネント
    lift_comp->SetConditionsForLifting(
        //ラムダ式を代入
        [this]() {
            if(lift_timer_ < 0) {
                return true;
            }
            return false;
        });
    lift_comp->SetConditionsForThrow(    //ラムダ式を代入
        [this]() {
            if(throw_timer_ < 0) {
                return true;
            }
            return false;
        });

    SetName(u8"エネミー");

    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Enemy::Update()
{
    __super::Update();
    //-------ここに区切られているものはAI出来たら消してください-------------
    squat_timer_--;
    jump_timer_--;
    face_down_timer_--;
    if(lifting_block_ == false)
        lift_timer_--;
    else
        throw_timer_--;
    //------------------------------------------------------------------
    //ジャンプをしていないなら
    if(squat_timer_ < 0 && GetComponent<ComponentJump>()->IsJumping() == false) {
        //ジャンプをできない状態にする
        GetComponent<ComponentJump>()->SetEnable();
        //高さを半径にする
        neutral_pos_ = SQUAT_TOP_POINT_;
        if(squat_timer_ < -RANDOM_TIME_MIN_) {
            squat_timer_ = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;
        }
    }
    else if(face_down_timer_ < 0 && GetComponent<ComponentJump>()->IsJumping() == false) {
        //ジャンプをできない状態にする
        GetComponent<ComponentJump>()->SetEnable();
        //高さを半径にする
        neutral_pos_ = FACE_DOWN_TOP_POINT_;
        //しゃがんでいると返す
        is_face_down_ = true;
        if(face_down_timer_ < -RANDOM_TIME_MIN_) {
            face_down_timer_ = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;
        }
    }
    else {
        //高さを半径の3倍にする
        neutral_pos_ = TOP_POINT_;
        //しゃがんでいないと返す
        is_face_down_ = false;
    }

    if(jump_timer_ < -RANDOM_TIME_MIN_) {
        jump_timer_ = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;    //AIができたら消してください
    }
    if(face_down_timer_ < -RANDOM_TIME_MIN_) {
        face_down_timer_ = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;    //AIができたら消してください
    }
    if(lift_timer_ < -RANDOM_TIME_MIN_) {
        lift_timer_    = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;    //AIができたら消してください
        lifting_block_ = true;
    }
    if(throw_timer_ < -RANDOM_TIME_MIN_) {
        throw_timer_   = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;    //AIができたら消してください
        lifting_block_ = false;
    }
    //コリジョンの高さの設定
    GetComponent<ComponentCollisionCapsule>()->SetHeight(RADIUS_ + neutral_pos_);
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void Enemy::Draw()
{
    float3 sphire_pos = float3(GetTranslate() + float3(0.0f, neutral_pos_, 0.0f));
    DrawSphere3D(cast(sphire_pos), RADIUS_, 16, WHITE, WHITE, TRUE);
    float3 cone_top    = float3(sphire_pos.xyz);
    float3 rot         = GetRotationAxisXYZ();
    float3 cone_bottom = float3(sphire_pos.x + (-5 * sinf(D2R(rot.y))), sphire_pos.y, sphire_pos.z + (-5 * cosf(D2R(rot.y))));
    DrawCone3D(cast(cone_bottom), cast(cone_top), RADIUS_, 16, WHITE, WHITE, TRUE);

    //うつ伏せの状態ではなかったら
    if(!is_face_down_) {
        //身体を表示する
        DrawCone3D(cast(sphire_pos), cast(GetTranslate()), RADIUS_, 16, WHITE, WHITE, TRUE);
    }

    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void Enemy::Exit()
{
    __super::Exit();
}

//!GUI表示
void Enemy::GUI()
{
    __super::GUI();
}
