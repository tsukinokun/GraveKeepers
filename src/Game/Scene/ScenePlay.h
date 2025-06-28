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
    const int ENEMY_MAX_     = 3;
    const int BLOCK_NUM_MAX_ = 10;
};
