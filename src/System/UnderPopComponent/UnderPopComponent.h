//---------------------------------------------------------------------------
//!	@file	ComponentFireBall.cpp
//! @brief	ファイアボールのコンポーネント
//! @auther 田中南々子
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(UnderPopComponent);

class UnderPopComponent : public Component
{
public:
    BP_COMPONENT_DECL(UnderPopComponent, u8"UnderPopComponent");

    void Init() override;
    void Update() override;    //!< 更新

private:
    const float DISTANCE_RANGE_X       = 40.0f;
    const float DISTANCE_RANGE_Z_MINUS = 25.5f;                             //Z方向の設置範囲(かぼちゃ方面)
    const float DISTANCE_RANGE_Z_PLUS  = DISTANCE_RANGE_Z_MINUS + 10.0f;    //Z方向の設置範囲(かぼちゃ方面)

    const float RADUIS_ = 2.0f;    //コリジョンの半径

    //０より下にいるかどうか
    bool is_under_ = true;
};
