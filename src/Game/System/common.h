//---------------------------------------------------------------------------
//!	@file	Common.h
//! @brief	共通する定数や列挙体をまとめたファイル
//---------------------------------------------------------------------------
constexpr int              HP_FONT_SIZE     = 50.0f;
const float                FIRST_HP_POS_X   = 100.0f - (float)HP_FONT_SIZE;
const float                WINDOW_W_HALF    = (float)WINDOW_W / 2;    // WINDOW_Wの1/2の位置
const float                WINDOW_W_QUARTER = (float)WINDOW_W / 4;    // WINDOW_Wの1/4の位置
const std::array<float, 4> HP_POS_X         = {
    FIRST_HP_POS_X + WINDOW_W_QUARTER * 0,
    FIRST_HP_POS_X + WINDOW_W_QUARTER * 1,
    FIRST_HP_POS_X + WINDOW_W_QUARTER * 2,
    FIRST_HP_POS_X + WINDOW_W_QUARTER * 3,
};    // HPのX軸の表示位置
const float                HP_POS_Y             = (float)(WINDOW_H - HP_FONT_SIZE * 2);    // HPのY軸の表示位置
const float                FIRST_MP_GAUGE_POS_X = FIRST_HP_POS_X + 85.0f;                  // MPゲージのX軸の表示位置はHPゲージに追従
const std::array<float, 4> MP_GAUGE_POS_X       = {
    FIRST_MP_GAUGE_POS_X + WINDOW_W_QUARTER * 0,
    FIRST_MP_GAUGE_POS_X + WINDOW_W_QUARTER * 1,
    FIRST_MP_GAUGE_POS_X + WINDOW_W_QUARTER * 2,
    FIRST_MP_GAUGE_POS_X + WINDOW_W_QUARTER * 3,
};
const float MP_GAUGE_POS_Y = HP_POS_Y + 80;              // MPゲージのY軸の表示位置
const float HP_BOX_H       = HP_POS_Y + HP_FONT_SIZE;    // HPの四角の高さ
const int   ENEMY_MAX      = 3;                          // エネミーの最大数
const int   CHARACTER_ALL  = ENEMY_MAX + 1;              // プレイヤーとエネミーの合計数
