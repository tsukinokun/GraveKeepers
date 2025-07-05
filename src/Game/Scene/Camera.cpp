//---------------------------------------------------------------------------
//!	@file	Camera.cpp
//! @brief	ゲームカメラ
//---------------------------------------------------------------------------
#include "Camera.h"
#include <System/Component/ComponentCollisionModel.h>
#include <System/Component/ComponentSpringArm.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Camera::Init()
{
    __super::Init();

    auto com_comp = AddComponent<ComponentCamera>();
    com_comp->SetPositionAndTarget({0, 50, 100}, {0, 0, 0});

    SetName(u8"Camera");
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Camera::Update()
{
    __super::Update();
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void Camera::Draw()
{
    __super::Draw();

    //フォントサイズの変更
    SetFontSize(SET_FONT_SIZE);
    ////文字の幅を取得
    auto player_str_nam = GetDrawFormatStringWidth("%3d", player_hp_, -1);
    ////文字数の真ん中
    float player_hp_str_half = player_str_nam / 2.0f;

    ////HPの描画座標
    float player_hp_pos_x = WINDOW_W_QUARTER - player_hp_str_half;

    //HPの表示
    DrawHP(player_hp_pos_x, player_hp_str_half);

    //フォントサイズを元に戻す
    SetFontSize(DEFAULT_FONT_SIZE);
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void Camera::Exit()
{
    __super::Exit();
}

//!GUI表示
void Camera::GUI()
{
    __super::GUI();
}

//!プレイヤーのHPを取得
void Camera::GetPlayerHP(int set_hp)
{
    player_hp_ = set_hp;
}

void Camera::DrawHP(float pos_x, float str)
{
    //HPの回りのボックスの幅
    float hp_box_pos_w = WINDOW_W_QUARTER + str;

    //HPの表示
    DrawFormatStringF(pos_x, HP_POS_Y, GetColor(0, 255, 255), "%3d", player_hp_);

    //HPの周りの四角(HPの左側に顔があるためボックスのスタートを左にずらす)
    DrawBoxAA(pos_x - str, HP_POS_Y, hp_box_pos_w, HP_BOX_H, GetColor(0, 0, 255), false);
}
