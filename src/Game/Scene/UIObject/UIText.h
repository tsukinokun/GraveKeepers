//---------------------------------------------------------------------------
//!	@file	UIText.h
//! @brief	UIの文字オブジェクト
//! @author
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
class ComponentString;    //ポインタとして使用するための前方宣言
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

public:
    std::weak_ptr<ComponentString>
        text_component_;    // 文字コンポーネントへの参照、使う側が毎回GetCompoenentするのも、weak_ptrを保持しておくのもさすがに面倒なので、ここでpublicにして保持しておく
};
