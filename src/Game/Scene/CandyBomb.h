//---------------------------------------------------------------------------
//!	@file	Block.h
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>

USING_PTR(CandyBomb);

class CandyBomb : public Object
{
public:
    BP_OBJECT_DECL(CandyBomb, u8"プレイシーンのキャンディー爆弾オブジェクト")

    //@
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

private:
    const float RADUIS_ = 2.0f;

    const float SCALE_ = 0.3f;

    const float DISTANCE_RANGE_ = 25.0f;
};
