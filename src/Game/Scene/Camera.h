//---------------------------------------------------------------------------
//!	@file	Camera.h
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#pragma once
//===========================================================================
//! アニメーションサンプルシーン
//===========================================================================
#include <System/Scene.h>

USING_PTR(Camera);

class Camera : public Object
{
public:
    BP_OBJECT_DECL(Camera, u8"プレイシーンのカメラ")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

    void GetPlayerHP(int set_hp);

private:
    const int FONT_SIZE        = 50;
    const int SET_FONT_SIZE    = 50;
    const int WINDOW_W_QUARTER = WINDOW_W / 4;

    int player_hp_ = 0;
};
