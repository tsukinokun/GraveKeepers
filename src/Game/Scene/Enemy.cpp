//---------------------------------------------------------------------------
//!	@file	GameMain.cpp
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#include "Enemy.h"
#include <System/Component/ComponentObjectController.h>
#include <System/Component/ComponentCollisionSphere.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Enemy::Init()
{
    __super::Init();
    SetTranslate({GetRand(PUT_RADIUS_MAX_) - PUT_RADIUS_MAX_ / 2, 2, GetRand(PUT_RADIUS_MAX_) - PUT_RADIUS_MAX_ / 2});
    auto col_comp = AddComponent<ComponentCollisionSphere>();
    col_comp->UseGravity();
    col_comp->SetRadius(RADIUS_);    // 球コリジョンの半径を3.0 にする
    SetName(u8"エネミー");
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Enemy::Update()
{
    __super::Update();
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void Enemy::Draw()
{
    float3 sphire_pos = float3(GetTranslate());
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
void Enemy::Exit()
{
    __super::Exit();
}

//!GUI表示
void Enemy::GUI()
{
    __super::GUI();
}
