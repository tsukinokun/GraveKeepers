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

    void GetPlayerHP(int set_hp);    //!< プレイヤーのHPを取得

private:
    void DrawHP(float pos_x, float str);    //!< HPの描画

    const int   SET_FONT_SIZE    = 50;              // フォントサイズ
    const float WINDOW_W_QUARTER = WINDOW_W / 4;    // WINDOW_Wの1/4の位置

    const float HP_POS_Y = WINDOW_H - SET_FONT_SIZE * 2;
    const float HP_BOX_H = HP_POS_Y + SET_FONT_SIZE;

    int player_hp_ = 0;    //プレイヤーのHP
};
