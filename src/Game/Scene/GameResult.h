//---------------------------------------------------------------------------
//!	@file	GameResult.h
//! @brief	リザルト画面
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>

//===========================================================================
//! アニメーションサンプルシーン
//===========================================================================
class GameResult : public Scene::Base
{
public:
    BP_CLASS_DECL(GameResult, u8"リザルト画面")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

    int result_back_graph_ = -1;    //!< リザルト背景画像
    int spider_web_graph_  = -1;    //!< 蜘蛛の巣画像

private:
};
