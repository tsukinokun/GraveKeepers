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

    void OnHit(const ComponentCollision::HitInfo& hit_info);

private:
    const float RADIUS_              = 2.0f;           //半径
    const float TOP_POINT_           = RADIUS_ * 3;    //しゃがんでいない頭の位置
    const float SQUAT_TOP_POINT_     = RADIUS_ * 2;    //しゃがんでいるときの頭の位置
    const float FACE_DOWN_TOP_POINT_ = RADIUS_;        //うつ伏せの時の頭の位置
    const float MOVE_SPEED_          = 0.1f;           //移動速度
    const int   PUT_RADIUS_MAX_      = 70;             //出現する範囲の最大
    const int   HP_MAX_              = 200;            //HPの最大値
    const int   TIMER_RANDOM_MAX_    = 5;              //タイマーのランダムの最大値

    float neutral_pos_  = TOP_POINT_;    //円の位置の高さ
    bool  is_face_down_ = false;         //うつ伏せの状態かを確認する用の変数

    const float1 LIFT_RANGE_  = 7.0f;    //持ち上げることができる距離
    const float1 THROW_RANGE_ = 7.0f;    //投げることができる距離

    bool set_lift_  = false;    //持ち上げるかどうかを決めるフラグ
    bool set_throw_ = false;    //持ち上げたものを投げるかどうかを決めるフラグ
    bool set_jump_  = false;    //ジャンプするかどうかを決めるフラグ

    float timer_count_ = static_cast<float>(GetRand(TIMER_RANDOM_MAX_) + 1);    //タイマー

    std::chrono::high_resolution_clock::time_point previous_time_;    // 前回の時間を記録する変数
};
