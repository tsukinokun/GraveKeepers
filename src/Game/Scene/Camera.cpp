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

    //文字の数を取得（HPが200を超えることがないため一番幅が大きいであろう8の3つでサイズ取得を代用）
    int hp_string_w = GetStringLength("888");
    //文字を半分ずらす
    int hp_string_w_half = hp_string_w / 2;
    int hp_string_h_half = SET_FONT_SIZE / 2;

    //HPの描画座標
    int player_hp_pos_x = WINDOW_W_QUARTER - SET_FONT_SIZE * hp_string_w_half;
    int player_hp_pos_y = WINDOW_H - SET_FONT_SIZE;

    //HPの回りのボックスの幅
    int player_hp_box_pos_w = WINDOW_W_QUARTER + SET_FONT_SIZE * hp_string_w_half;
    int player_hp_box_pos_h = player_hp_pos_y + SET_FONT_SIZE;

    //フォントサイズの変更
    SetFontSize(SET_FONT_SIZE);
    //HPの表示
    DrawFormatString(player_hp_pos_x, player_hp_pos_y, GetColor(0, 255, 255), "%3d", player_hp_);

    DrawBox(player_hp_pos_x, player_hp_pos_y, player_hp_box_pos_w, player_hp_box_pos_h, GetColor(0, 0, 255), false);

    //DrawLine(WINDOW_W_QUARTER, 0, WINDOW_W_QUARTER, WINDOW_H, GetColor(255, 0, 255));

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
