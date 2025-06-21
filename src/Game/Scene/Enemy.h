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
    const float RADIUS_         = 2.0f;           //半径
    const float TOP_POINT_      = RADIUS_ * 3;    //しゃがんでいない頭の位置
    const int   PUT_RADIUS_MAX_ = 50;             //出現する範囲の最大

    float neutral_pos_ = TOP_POINT_;    //円の位置の高さ

    //以下のものはAIができたら消してください
    const int SQUAT_TIME_MIN_ = 60;                      //しゃがむ時間の最小時間
    const int SQUAT_TIME_MAX_ = SQUAT_TIME_MIN_ * 10;    //しゃがむ時間の最大時間

    int squat_timer_;    //しゃがむ時間
    int jump_timer_;     //ジャンプする時間
};
