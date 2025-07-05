//---------------------------------------------------------------------------
//!	@file	Player.h
//! @brief	プレイヤーヘッダー
//---------------------------------------------------------------------------
#pragma once
//===========================================================================
//! アニメーションサンプルシーン
//===========================================================================
#include <System/Scene.h>

USING_PTR(Player);

class Player : public Object
{
public:
    BP_OBJECT_DECL(Player, u8"プレイシーンのプレイヤー")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

    int GetHP();    //!<HPの値を渡す関数

    void OnHit(const ComponentCollision::HitInfo& hit_info) override;

private:
    const float RADIUS_              = 2.0f;           //半径
    const float TOP_POINT_           = RADIUS_ * 3;    //しゃがんでいない頭の位置
    const float SQUAT_TOP_POINT_     = RADIUS_ * 2;    //しゃがんでいるときの頭の位置
    const float FACE_DOWN_TOP_POINT_ = RADIUS_;        //うつ伏せの時の頭の位置
    const int   HP_MAX_              = 200;            //HPの最大値

    float neutralpos_   = TOP_POINT_;    //円の位置の高さ
    bool  is_face_down_ = false;         //うつ伏せの状態かを確認する用の変数
    int   hp_           = 0;             //HP用の変数
};
