//---------------------------------------------------------------------------
//!	@file	ScenePlay.h
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>

//===========================================================================
//! アニメーションサンプルシーン
//===========================================================================
class ScenePlay : public Scene::Base
{
public:
    BP_CLASS_DECL(ScenePlay, u8"インゲームのシーン")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

private:
    float                                          TIMER_COUNT_ = 120.0f;
    const int                                      ENEMY_MAX_   = 3;
    std::chrono::high_resolution_clock::time_point previous_time_;
    const int                                      BLOCK_NUM_MAX_ = 10;
    //const int                                      CANDYBOMB_NUM_MAX_ = 5;

    const std::array<float3, 4> WALL_POS_ = {float3(50.0f, 0.0f, 0.0f), float3(-50.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 50.0f), float3(0.0f, 0.0f, -50.0f)};
};
