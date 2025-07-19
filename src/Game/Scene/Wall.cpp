//---------------------------------------------------------------------------
//!	@file	Wall.cpp
//! @brief	壁
//! @author 山崎愛
//---------------------------------------------------------------------------
#include "Wall.h"
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionModel.h>
#include <System/Component/ComponentLiftable.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Wall::Init()
{
    __super::Init();

    SetName(u8"Wall");
    SetRotationAxisXYZ(float3(0.0f, 0.0f, 90.0f));
    //フィールドの読み込み（仮で山口先生の素材を入れている）
    auto model_comp = AddComponent<ComponentModel>("data/Sample/PoyPoy/Field/field.mv1");
    model_comp->SetScaleAxisXYZ(float3(1.0f, 1.0f, 2.0f));
    model_comp->SetStatus(Component::StatusBit::NoDraw, true);    //描画をオフにする
    AddComponent<ComponentCollisionModel>()->AttachToModel();     //< GUIでの 「モデルにコリジョンを張り付ける」動作
    //座標の設定
    SetTranslate(float3(0.0f, 0.0f, 0.0f));
    //モデルを１０倍している
    SetScaleAxisXYZ(10.0f);
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Wall::Update()
{
    __super::Update();
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void Wall::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void Wall::Exit()
{
    __super::Exit();
}

//!GUI表示
void Wall::GUI()
{
    __super::GUI();
}
