//---------------------------------------------------------------------------
//!	@file	HlslppUseful.cpp
//! @brief	Hlslppの拡張
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
//---------------------------------------------------------------------------
//	マウス座標を float2 で取得
//! @retval マウスのスクリーン座標
//---------------------------------------------------------------------------
float2 GetMouseFloat2();

//---------------------------------------------------------------------------
//	2Dの四角同士の当たり判定（座標と大きさに Float2 を渡すバージョン）
//! @param box_pos1 [in] 四角1の座標
//! @param box_size1 [in] 四角1の大きさ
//! @param box_pos2 [in] 四角2の座標
//! @param box_size2 [in] 四角2の大きさ
//! @retval 当たっているかどうか
//---------------------------------------------------------------------------
bool CheckBoxCenterHit(const float2& box_pos1, const float2& box_size1, const float2& box_pos2, const float2& box_size2);

//---------------------------------------------------------------------------
//	2Dの四角と点の当たり判定（座標と大きさに Float2 を渡すバージョン）
//! @param box_pos [in] 四角の座標
//! @param box_size [in] 四角の大きさ
//! @param point [in] 点の座標
//! @retval 当たっているかどうか
//---------------------------------------------------------------------------
bool CheckBoxPointHit(const float2& box_pos, const float2& box_size, const float2& point);
//---------------------------------------------------------------------------
// 円形に配置した座標を返す関数
//! @params center 円の中心座標
//! @params radius 円の半径
//! @params divisions 円周上の分割数
//! @params angleOffset 角度のオフセット（度数）
//! @params index 取得したい点のインデックス（0からdivisions-1まで）
//---------------------------------------------------------------------------
float3 GetPointOnCircle(const float3& center, float radius, int divisions, int index, float angle_offset = 0.0f);
//---------------------------------------------------------------------------
// 複数の点の中心を求める関数
//! @param points [in] 座標のベクター
//! @retval 中心座標
//---------------------------------------------------------------------------
float3 CalculateCenter(const std::vector<float3>& points);
