//---------------------------------------------------------------------------
//!	@file	SceneTitle.h
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
//===========================================================================
//! アニメーションサンプルシーン
//===========================================================================
class SceneTitle : public Scene::Base
{
public:
    BP_CLASS_DECL(SceneTitle, u8"タイトルのシーン")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

private:
};
