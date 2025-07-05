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

    void GetPlayerHP(int set_hp);                  //!< プレイヤーのHPを取得
    void GetEnemyHP(int set_hp, int enemy_num);    //!< エネミーのHPを取得

private:
    void DrawHP(float pos_x, float str_half, int str, int hp, int character);    //!< HPの描画

    const int SET_FONT_SIZE = 50;    // フォントサイズ

    const float WINDOW_W_HALF    = (float)WINDOW_W / 2;    // WINDOW_Wの1/2の位置
    const float WINDOW_W_QUARTER = (float)WINDOW_W / 4;    // WINDOW_Wの1/4の位置
    const float WINDOW_W_EIGHTH  = (float)WINDOW_W / 8;    // WINDOW_Wの1/8の位置

    const float HP_POS_Y       = (float)(WINDOW_H - SET_FONT_SIZE * 2);    // HPのY軸の表示位置
    const float HP_BOX_H       = HP_POS_Y + SET_FONT_SIZE;                 // HPの四角の高さ
    const int   ENEMY_MAX_     = 3;                                        // エネミーの最大数
    const int   CHARACTER_ALL_ = ENEMY_MAX_ + 1;                           // プレイヤーとエネミーの合計数

    int player_hp_   = 0;      //プレイヤーのHP
    int enemy_hp_[3] = {0};    //エネミーのHP

    int box_color_[4]  = {0};    //HPの色を変えるための変数
    int font_color_[4] = {0};    //HPの色を変えるための変数
};
