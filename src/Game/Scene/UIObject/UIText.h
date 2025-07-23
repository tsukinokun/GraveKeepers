//---------------------------------------------------------------------------
//!	@file	UIText.h
//! @brief	UIの文字オブジェクト
//! @author
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>

USING_PTR(UIText);

class UIText : public Object
{
public:
    BP_OBJECT_DECL(UIText, u8"UIの文字オブジェクト")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示
};
