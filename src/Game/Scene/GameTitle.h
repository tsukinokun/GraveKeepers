//---------------------------------------------------------------------------
//!	@file	GameTitle.h
//! @brief	タイトル画面
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>

//===========================================================================
//! アニメーションサンプルシーン
//===========================================================================
class GameTitle : public Scene::Base
{
public:
    BP_CLASS_DECL(GameTitle, u8"タイトル画面")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

    int titleBackGraph = -1;    //!< タイトル背景画像ハンドル

private:
};
