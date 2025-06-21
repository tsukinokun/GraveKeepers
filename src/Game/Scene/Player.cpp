//---------------------------------------------------------------------------
//!	@file	GameMain.cpp
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#include "Player.h"
#include <System/Component/ComponentObjectController.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentJump.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Player::Init()
{
    __super::Init();
    auto object_controller_comp = AddComponent<ComponentObjectController>();
    object_controller_comp->SetMoveSpeed(0.2f);
    object_controller_comp->SetRotateSpeed(20.0f);
    SetTranslate({0, 2, 0});
    auto col_comp = AddComponent<ComponentCollisionCapsule>();
    col_comp->UseGravity();
    col_comp->SetRadius(RADIUS_);             // 球コリジョンの半径を2.0 にする
    col_comp->SetHeight(RADIUS_ + hight_);    // 球コリジョンの高さを半径の４倍 にする
    auto jump_comp = AddComponent<ComponentJump>();
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
    if(CheckHitKey(KEY_INPUT_DOWN) && GetComponent<ComponentJump>()->IsJump() == false) {
        //ジャンプをできない状態にする
        GetComponent<ComponentJump>()->NotJump();
        //高さを半径にする
        hight_ = RADIUS_;
    }
    else {
        //高さを半径の3倍にする
        hight_ = RADIUS_ * 3;
    }
    //コリジョンの高さの設定
    GetComponent<ComponentCollisionCapsule>()->SetHeight(RADIUS_ + hight_);
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void Player::Draw()
{
    float3 sphire_pos = float3(GetTranslate() + float3(0.0f, hight_, 0.0f));
    DrawSphere3D(cast(sphire_pos), RADIUS_, 16, WHITE, WHITE, TRUE);
    float3 cone_top    = float3(sphire_pos.xyz);
    float3 rot         = GetRotationAxisXYZ();
    float3 cone_bottom = float3(sphire_pos.x + (-5 * sinf(D2R(rot.y))), sphire_pos.y, sphire_pos.z + (-5 * cosf(D2R(rot.y))));
    DrawCone3D(cast(cone_bottom), cast(cone_top), RADIUS_, 16, WHITE, WHITE, TRUE);
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
