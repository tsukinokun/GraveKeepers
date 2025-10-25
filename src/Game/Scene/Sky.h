//---------------------------------------------------------------------------
//!	@file	Field.h
//! @brief	フィールドヘッダー
//---------------------------------------------------------------------------
#pragma once
//===========================================================================
//! アニメーションサンプルシーン
//===========================================================================
#include <System/Scene.h>

USING_PTR(Sky);

class Sky : public Object
{
public:
    BP_OBJECT_DECL(Sky, u8"プレイシーンの空")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

private:
};
