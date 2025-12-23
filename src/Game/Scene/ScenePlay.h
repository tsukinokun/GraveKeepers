//---------------------------------------------------------------------------
//!	@file	ScenePlay.h
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <array>
#include <Game/Scene/info/ResultInfo.h>
class Character;    // 前方宣言

//---------------------------------------------------------------------------
//! @brief	インゲームのシーンクラス
//---------------------------------------------------------------------------
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
    float                                          TIMER_COUNT_ = 5.0f;
    const int                                      ENEMY_MAX_   = 3;
    std::chrono::high_resolution_clock::time_point previous_time_;
    const int                                      BLOCK_NUM_MAX_ = 10;
    //const int                                      CANDYBOMB_NUM_MAX_ = 5;

    const std::array<float3, 4> WALL_POS_ = {float3(70.0f, 0.0f, 0.0f), float3(-70.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 70.0f), float3(0.0f, 0.0f, -70.0f)};
    std::vector<std::weak_ptr<Character>> characters_;      //!< キャラクターオブジェクトの配列
    std::array<ResultInfo, 4>             result_datas_;    //!< リザルト情報
};
