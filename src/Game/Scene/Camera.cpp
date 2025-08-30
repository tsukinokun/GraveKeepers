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

    for(int i = 0; i < CHARACTER_ALL_; i++) {
        box_color_[i]  = GetColor(0, 0, 255);      //プレイヤーのHPの色
        font_color_[i] = GetColor(0, 255, 255);    //プレイヤーのHPの色
    }
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

    ////フォントサイズの変更
    //SetFontSize(SET_FONT_SIZE);
    ////文字の幅を取得
    //auto player_str_nam = GetDrawFormatStringWidth("%3d", player_hp_, -1);
    ////文字数の真ん中
    //float player_hp_str_half = player_str_nam / 2.0f;

    //////HPの描画座標
    //float player_hp_pos_x = WINDOW_W_QUARTER - player_hp_str_half;

    ////HPの表示
    //DrawHP(player_hp_pos_x, player_hp_str_half, player_str_nam, player_hp_, 1);

    //for(int i = 0; i < ENEMY_MAX_; i++)
    //{
    //	//エネミーのHPの文字の長さ
    //	auto enemy_str_nam = GetDrawFormatStringWidth("%3d", enemy_hp_[i], -1);
    //	//エネミーのHPの文字の半分
    //	float enemy_hp_str_half = enemy_str_nam / 2.0f;
    //	//エネミーのHPのX軸の表示位置
    //	float enemy_hp_pos_x = 0;

    //	if(i == 0)	  //エネミーの体力表示が画面半分より左だったら
    //		enemy_hp_pos_x = WINDOW_W_HALF - WINDOW_W_EIGHTH;
    //	else	// エネミーの体力表示が画面半分より右だったら
    //		enemy_hp_pos_x = WINDOW_W_HALF + WINDOW_W_EIGHTH * i;

    //	//エネミーのHPの表示
    //	DrawHP(enemy_hp_pos_x, enemy_hp_str_half, enemy_str_nam, enemy_hp_[i], i + 1);
    //}
    ////フォントサイズを元に戻す
    //SetFontSize(DEFAULT_FONT_SIZE);

    ////プレイヤー含めての数
    //for(int i = 0; i < CHARACTER_ALL_; i++)
    //{
    //	//何Pかを表示するためのY軸の位置
    //	float pos_y = HP_POS_Y - DEFAULT_FONT_SIZE / 2;
    //	//プレイヤーとエネミーの一体目
    //	if(i < CHARACTER_ALL_ / 2)
    //	{
    //		//何Pかを表示するためのX軸の位置
    //		float pos_x = WINDOW_W_HALF - WINDOW_W_EIGHTH - player_hp_str_half * (i + 1);
    //		//文字の描画
    //		DrawFormatStringF(pos_x - WINDOW_W_EIGHTH * i, pos_y, GetColor(255, 255, 0), "%1dP", CHARACTER_ALL_ / 2 - i);
    //	}
    //	//エネミーの三体目と四体目
    //	else
    //	{
    //		//何Pかを表示するためのX軸の位置
    //		float pos_x = WINDOW_W_HALF + WINDOW_W_EIGHTH - player_hp_str_half;
    //		//文字の描画
    //		DrawFormatStringF(pos_x + WINDOW_W_EIGHTH * (i - CHARACTER_ALL_ / 2), pos_y, GetColor(255, 255, 0), "%1dP", i + 1);
    //	}
    //}
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

void Camera::GetEnemyHP(int set_hp, int enemy_num)
{
    enemy_hp_[enemy_num] = set_hp;
}

void Camera::DrawHP(float pos_x, float str_half, int str, int hp, int character)
{
    //HPの回りのボックスの幅
    float hp_box_pos_w = pos_x + str;

    //HPの表示
    DrawFormatStringF(pos_x, HP_POS_Y, font_color_[character], "%3d", hp);

    //HPの周りの四角(HPの左側に顔があるためボックスのスタートを左にずらす)
    DrawBoxAA(pos_x - str_half, HP_POS_Y, hp_box_pos_w, HP_BOX_H, box_color_[character], false);
}
