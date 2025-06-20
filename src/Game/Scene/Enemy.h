//---------------------------------------------------------------------------
//!	@file	Enemy.h
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#pragma once
//===========================================================================
//! アニメーションサンプルシーン
//===========================================================================
#include <System/Scene.h>

USING_PTR(Enemy);

class Enemy : public Object
{
public:
    BP_OBJECT_DECL(Enemy, u8"プレイシーンのエネミー")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

private:
    const float RADIUS_         = 2.0f;    //半径
    const int   PUT_RADIUS_MAX_ = 50;
};
