//---------------------------------------------------------------------------
//!	@file	Player.h
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#pragma once
//===========================================================================
//! アニメーションサンプルシーン
//===========================================================================
#include <System/Scene.h>

USING_PTR(Player);

class Player : public Object
{
public:
    BP_OBJECT_DECL(Player, u8"プレイシーンのプレイヤー")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

private:
    const float RADIUS_ = 2.0f;    //半径
};
