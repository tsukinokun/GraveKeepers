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

    //当たり判定のコールバック関数
    //! @param hit_info[in] ヒットした相手側の情報
    void OnHit(const ComponentCollision::HitInfo& hit_info) override;

private:
    const float RADUIS_ = 2.0f;     //コリジョンの半径
    const float HEIGHT_ = 10.0f;    //コリジョンの高さ

    const float SCALE_ = 1.0f;    //サイズ

    const float DISTANCE_RANGE_ = 25.0f;    //設置範囲

    const float POP_HEIGHT_MIN_ = -9.0f;    //生成時の最低高さ
    const float POP_HEIGHT_MAX_ = -3.0f;    //生成時の最高高さ

    bool is_rising_ = true;

    enum
    {
        CrossGrave,
        FlatGrave,

        BlockTypeMax,
    };
};
