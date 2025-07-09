//---------------------------------------------------------------------------
//!	@file	Block.cpp
//! @brief	ゲームシーンのブロックオブジェクト
//---------------------------------------------------------------------------
#include "Block.h"
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentRigidbody.h>
#include <System/Component/ComponentLiftable.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Block::Init()
{
    __super::Init();

    SetName(u8"ブロック");
    auto rb = AddComponent<ComponentRigidbody>();
    AddComponent<ComponentLiftable>();    //持ち上げられ機能コンポーネント
    auto block_com = AddComponent<ComponentCollisionCapsule>();
    //block_com->UseGravity();
    block_com->SetRadius(RADUIS_);
    SetTranslate(float3(0.0f, 0.0f, 10.0f));
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Block::Update()
{
    __super::Update();
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void Block::Draw()
{
    __super::Draw();
    float3 sphir_pos = float3(GetTranslate() + float3(0.0f, RADUIS_, 0.0f));
    DrawSphere3D(cast(sphir_pos), RADUIS_, 16, GREEN, GREEN, TRUE);
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void Block::Exit()
{
    __super::Exit();
}

//!GUI表示
void Block::GUI()
{
    __super::GUI();
}
