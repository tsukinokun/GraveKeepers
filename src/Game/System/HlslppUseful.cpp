//---------------------------------------------------------------------------
//!	@file	HlslppUseful.cpp
//! @brief	Hlslppの拡張
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "HlslppUseful.h"

//---------------------------------------------------------------------------------
//!	マウス座標を float2 で取得
//---------------------------------------------------------------------------------
float2 GetMouseFloat2()
{
    float2 pos;
    pos.x = (float)GetMouseX();
    pos.y = (float)GetMouseY();
    return pos;
}

//---------------------------------------------------------------------------
//	2Dの四角同士の当たり判定（座標と大きさに Float2 を渡すバージョン）
//---------------------------------------------------------------------------
bool CheckBoxCenterHit(const float2& box_pos1, const float2& box_size1, const float2& box_pos2, const float2& box_size2)
{
    if(box_pos1.x + box_size1.x / 2 >= box_pos2.x - box_size2.x / 2 && box_pos1.x - box_size1.x / 2 <= box_pos2.x + box_size2.x / 2) {
        if(box_pos1.y + box_size1.y / 2 >= box_pos2.y - box_size2.y / 2 && box_pos1.y - box_size1.y / 2 <= box_pos2.y + box_size2.y / 2) {
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------
//	2Dの四角と点の当たり判定（座標と大きさに Float2 を渡すバージョン）
//---------------------------------------------------------------------------
bool CheckBoxPointHit(const float2& box_pos, const float2& box_size, const float2& point)
{
    if(box_pos.x + box_size.x / 2 >= point.x && box_pos.x - box_size.x / 2 <= point.x) {
        if(box_pos.y + box_size.y / 2 >= point.y && box_pos.y - box_size.y / 2 <= point.y) {
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------
//! @brief 円形に配置した座標を返す関数
//---------------------------------------------------------------------------
float3 GetPointOnCircle(const float3& center, float radius, int divisions, int index, float angle_offset)
{
    if(divisions <= 0)
        return center;                             // 安全対策
    float angle_offset_rad = D2R(angle_offset);    // 角度のオフセットをラジアンに変換
    // インデックスに対応する角度を計算
    float theta = 2.0f * PI * index / divisions + angle_offset_rad;

    // y軸を固定し、x-z平面に円を描く
    float x = center.x + radius * std::cos(theta);
    float y = center.y;
    float z = center.z + radius * std::sin(theta);

    return float3(x, y, z);
}

//---------------------------------------------------------------------------
// 複数の点の中心を求める関数
//---------------------------------------------------------------------------
float3 CalculateCenter(const std::vector<float3>& points)
{
    // 点が存在しない場合は原点を返す
    if(points.empty())
        return float3(0, 0, 0);
    // 点の合計値を宣言
    float3 sum(0, 0, 0);
    // 全ての点を合計
    for(const auto& p : points) {
        sum += p;
    }
    // 合計を点の数で割って中心を求める
    return sum / static_cast<float>(points.size());
}
