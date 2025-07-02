//---------------------------------------------------------------------------
//!	@file	Player.cpp
//! @brief	プレイヤー
//---------------------------------------------------------------------------
#include "Player.h"
#include <System/Component/ComponentObjectController.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentJump.h>
#include <System/Component/ComponentLift.h>
#include <System/Component/ComponentRigidbody.h>
#include <System/Component/ComponentLiftable.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Player::Init()
{
    __super::Init();
    AddComponent<ComponentRigidbody>();    //剛体コンポーネントを追加
    AddComponent<ComponentLiftable>();     //持ち上げられ機能コンポーネント
    auto object_controller_comp = AddComponent<ComponentObjectController>();
    object_controller_comp->SetMoveSpeed(0.2f);
    object_controller_comp->SetRotateSpeed(20.0f);
    SetTranslate({0, 2, 0});
    auto col_comp = AddComponent<ComponentCollisionCapsule>();
    col_comp->UseGravity();
    col_comp->SetRadius(RADIUS_);                  // 球コリジョンの半径を2.0 にする
    col_comp->SetHeight(RADIUS_ + neutralpos_);    // 球コリジョンの高さを半径の４倍 にする

    auto jump_comp = AddComponent<ComponentJump>();

    jump_comp->SetConditionsJump([]() {
        if(IsKeyOn(KEY_INPUT_SPACE))
            return true;
        return false;
    });

    col_comp->SetCollisionGroup(ComponentCollision::CollisionGroup::PLAYER);    // 所属するグループを「PLAYER」とします
    auto lift_comp = AddComponent<ComponentLift>();                             //持ち上げコンポーネント
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

    SetName(u8"プレイヤー");
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Player::Update()
{
    __super::Update();
    //下キーを押しているかつジャンプをしていないなら
    if(!GetComponent<ComponentLiftable>()->IsLifted()) {
        if(CheckHitKey(KEY_INPUT_DOWN) && GetComponent<ComponentJump>()->IsJumping() == false) {
            //ジャンプをできない状態にする
            GetComponent<ComponentJump>()->SetEnable();
            //高さを半径にする
            neutralpos_ = SQUAT_TOP_POINT_;
        }
        //右のシフトキーを押しているかつジャンプをしていないなら
        else if(CheckHitKey(KEY_INPUT_RSHIFT) && GetComponent<ComponentJump>()->IsJumping() == false) {
            //ジャンプをできない状態にする
            GetComponent<ComponentJump>()->SetEnable();
            //高さを半径にする
            neutralpos_ = FACE_DOWN_TOP_POINT_;
            //しゃがんでいると返す
            is_face_down_ = true;
        }
        //上の状態でなかったら
        else {
            //高さを半径の3倍にする
            neutralpos_ = TOP_POINT_;
            //しゃがんでいないと返す
            is_face_down_ = false;
        }
    }
    else {
        //高さを半径の3倍にする
        neutralpos_ = TOP_POINT_;
        //しゃがんでいないと返す
        is_face_down_ = false;
    }
    //コリジョンの高さの設定
    GetComponent<ComponentCollisionCapsule>()->SetHeight(RADIUS_ + neutralpos_);
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void Player::Draw()
{
    float3 sphire_pos = float3(GetTranslate() + float3(0.0f, neutralpos_, 0.0f));
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
void Player::Exit()
{
    __super::Exit();
}

//!GUI表示
void Player::GUI()
{
    __super::GUI();
}
