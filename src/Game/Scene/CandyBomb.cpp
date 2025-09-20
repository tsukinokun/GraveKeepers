//---------------------------------------------------------------------------
//!	@file	Block.cpp
//! @brief	ゲームシーンのブロックオブジェクト
//---------------------------------------------------------------------------
#include "CandyBomb.h"
#include <System/RandomRange/RandomRange.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentRigidbody.h>
#include <System/Component/ComponentLiftable.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool CandyBomb::Init()
{
    __super::Init();

    SetName(u8"キャンディー爆弾");

    //球のコリジョンをアタッチ（モデルの形通りにするとすり抜けたため形通りにはしていません）
    auto candy_col = AddComponent<ComponentCollisionSphere>();
    //block_col->UseGravity();
    // コリジョンの半径を設定
    candy_col->SetRadius(RADUIS_);

    AddComponent<ComponentModel>("data/PoyPoy/Model/Object/CandyBomb/candy.mv1");

    //モデルコンポーネントを取得
    auto obj = GetComponent<ComponentModel>();
    obj->SetScaleAxisXYZ(SCALE_);    //サイズの設定
    //位置の設定（-DISTANCE_RANGE_からDISTANCE_RANGE_の間に設置）
    SetTranslate(float3(GetRandomRangeF(-DISTANCE_RANGE_, DISTANCE_RANGE_), 0.0f, GetRandomRangeF(-DISTANCE_RANGE_, DISTANCE_RANGE_)));

    auto rb = AddComponent<ComponentRigidbody>();
    AddComponent<ComponentLiftable>();    //持ち上げられ機能コンポーネント

    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void CandyBomb::Update()
{
    __super::Update();
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void CandyBomb::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void CandyBomb::Exit()
{
    __super::Exit();
}

//!GUI表示
void CandyBomb::GUI()
{
    __super::GUI();
}
