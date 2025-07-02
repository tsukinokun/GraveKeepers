//---------------------------------------------------------------------------
//!	@file	Field.h
//! @brief	フィールドヘッダー
//---------------------------------------------------------------------------
#pragma once
//===========================================================================
//! アニメーションサンプルシーン
//===========================================================================
#include <System/Scene.h>

USING_PTR(Field);

class Field : public Object
{
public:
    BP_OBJECT_DECL(Field, u8"プレイシーンのフィールド")

    //@}
    bool Init() override;                                                //!< 初期化
    void Update() override;                                              //!< 更新
    void Draw() override;                                                //!< 描画
    void Exit() override;                                                //!< 終了
    void GUI() override;                                                 //!< GUI表示
    void OnHit(const ComponentCollision::HitInfo& hit_info) override;    //!< ヒット時処理

private:
};
