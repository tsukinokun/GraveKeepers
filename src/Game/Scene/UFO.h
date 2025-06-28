//---------------------------------------------------------------------------
//!	@file	UFO.h
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#pragma once
//===========================================================================
//! アニメーションサンプルシーン
//===========================================================================
#include <System/Scene.h>

USING_PTR(UFO);

class UFO : public Object
{
public:
    BP_OBJECT_DECL(UFO, u8"プレイシーンのUFO")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

private:
};
