//---------------------------------------------------------------------------
//!	@file	GameMain.cpp
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#include "Player.h"
#include <System/Component/ComponentObjectController.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Player::Init()
{
    __super::Init();
    auto object_controller_comp = AddComponent<ComponentObjectController>();
    object_controller_comp->SetMoveSpeed(0.5f);
    object_controller_comp->SetRotateSpeed(20.0f);
    SetTranslate({0, 0, 0});
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
    float3 sphire_pos = float3(GetTranslate());
    DrawSphere3D(cast(sphire_pos), RADIUS_, 16, WHITE, WHITE, TRUE);
    float3 cone_top    = float3(sphire_pos.xyz);
    float3 rot         = GetRotationAxisXYZ();
    float3 cone_bottom = float3(sphire_pos.x + (-10 * sinf(D2R(rot.y))), sphire_pos.y, sphire_pos.z + (-10 * cosf(D2R(rot.y))));
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
