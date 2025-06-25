//---------------------------------------------------------------------------
//!	@file	Enemy.h
//! @brief	エネミーヘッダー
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
    const float RADIUS_              = 2.0f;           //半径
    const float TOP_POINT_           = RADIUS_ * 3;    //しゃがんでいない頭の位置
    const float SQUAT_TOP_POINT_     = RADIUS_ * 2;    //しゃがんでいるときの頭の位置
    const float FACE_DOWN_TOP_POINT_ = RADIUS_;        //うつ伏せの時の頭の位置
    const int   PUT_RADIUS_MAX_      = 50;             //出現する範囲の最大

    float neutral_pos_  = TOP_POINT_;    //円の位置の高さ
    bool  is_face_down_ = false;         //うつ伏せの状態かを確認する用の変数

    //以下のものはAIができたら消してください
    const int RANDOM_TIME_MIN_ = 60;                       //しゃがむ時間の最小時間
    const int RANDOM_TIME_MAX_ = RANDOM_TIME_MIN_ * 10;    //しゃがむ時間の最大時間

    int squat_timer_;        //しゃがむ時間
    int jump_timer_;         //ジャンプする時間
    int face_down_timer_;    //ジャンプする時間
};
