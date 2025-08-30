//---------------------------------------------------------------------------
//!	@file	Common.h
//! @brief	共通する定数や列挙体をまとめたファイル
//---------------------------------------------------------------------------
constexpr int            HP_FONT_SIZE     = 50.0f;
const float              FIRST_HP_POS_X   = 200.0f;
const float              WINDOW_W_HALF    = (float)WINDOW_W / 2;    // WINDOW_Wの1/2の位置
const float              WINDOW_W_QUARTER = (float)WINDOW_W / 4;    // WINDOW_Wの1/4の位置
const std::array<int, 4> HP_POS_X         = {
    FIRST_HP_POS_X + WINDOW_W_QUARTER * 0 - HP_FONT_SIZE,
    FIRST_HP_POS_X + WINDOW_W_QUARTER * 1 - HP_FONT_SIZE,
    FIRST_HP_POS_X + WINDOW_W_QUARTER * 2 - HP_FONT_SIZE,
    FIRST_HP_POS_X + WINDOW_W_QUARTER * 3 - HP_FONT_SIZE,
};    // HPのX軸の表示位置
const float HP_POS_Y      = (float)(WINDOW_H - HP_FONT_SIZE * 2);    // HPのY軸の表示位置
const float HP_BOX_H      = HP_POS_Y + HP_FONT_SIZE;                 // HPの四角の高さ
const int   ENEMY_MAX     = 3;                                       // エネミーの最大数
const int   CHARACTER_ALL = ENEMY_MAX + 1;                           // プレイヤーとエネミーの合計数
