//---------------------------------------------------------------------------
//!	@file	UFO.cpp
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#include "UFO.h"
#include <cmath>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool UFO::Init()
{
    __super::Init();
    SetName(u8"UFO");
    SetTranslate({0.0f, 35.0f, 0.0f});
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void UFO::Update()
{
    __super::Update();
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void UFO::Draw()
{
    __super::Draw();
    auto pos   = GetTranslate();
    auto scale = GetScaleAxisXYZ();

    auto size  = 5;
    auto hight = 0.5f;

    float3 p1 = pos + float3(size, 0, 0) * scale;
    float3 p2 = pos + float3(-size, 0, 0) * scale;
    float3 p3 = pos + float3(0, 0, size) * scale;
    float3 p4 = pos + float3(0, 0, -size) * scale;

    float3 p5 = pos + float3(size, hight, 0) * scale;
    float3 p6 = pos + float3(-size, hight, 0) * scale;
    float3 p7 = pos + float3(0, hight, size) * scale;
    float3 p8 = pos + float3(0, hight, -size) * scale;

    auto color = GetColor(211, 211, 211);

    //上下の四角
    DrawTriangle3D(cast(p1), cast(p2), cast(p3), color, true);
    DrawTriangle3D(cast(p1), cast(p2), cast(p4), color, true);

    DrawTriangle3D(cast(p5), cast(p6), cast(p7), color, true);
    DrawTriangle3D(cast(p5), cast(p6), cast(p8), color, true);

    //横の四角
    DrawTriangle3D(cast(p1), cast(p3), cast(p5), color, true);
    DrawTriangle3D(cast(p1), cast(p4), cast(p5), color, true);

    DrawTriangle3D(cast(p3), cast(p5), cast(p7), color, true);
    DrawTriangle3D(cast(p4), cast(p5), cast(p8), color, true);

    DrawTriangle3D(cast(p2), cast(p3), cast(p7), color, true);
    DrawTriangle3D(cast(p2), cast(p4), cast(p8), color, true);

    DrawTriangle3D(cast(p2), cast(p6), cast(p7), color, true);
    DrawTriangle3D(cast(p2), cast(p6), cast(p8), color, true);
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void UFO::Exit()
{
    __super::Exit();
}

//!GUI表示
void UFO::GUI()
{
    __super::GUI();
}
