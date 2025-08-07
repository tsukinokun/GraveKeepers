//---------------------------------------------------------------------------
//!	@file	Block.h
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>

USING_PTR(Block);

class Block : public Object
{
public:
    BP_OBJECT_DECL(Block, u8"プレイシーンのブロックオブジェクト")

    //@
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

private:
    const float RADUIS_ = 2.0f;

    const float SCALE_ = 1.0f;

    const float DISTANCE_RANGE_ = 25.0f;

    enum
    {
        CrossGrave,
        FlatGrave,

        BlockTypeMax,
    };
};
