//---------------------------------------------------------------------------
//!	@file	Player.h
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#pragma once
//===========================================================================
//! アニメーションサンプルシーン
//===========================================================================
#include <System/Scene.h>

USING_PTR(UFO);

class UFO : public Object
{
public:
    BP_OBJECT_DECL(UFO, u8"プレイシーンのＵＦＯ")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

private:
    const float RADIUS_ = 2.0f;    //半径

    float hight_ = RADIUS_ * 3;
};
