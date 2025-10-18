//---------------------------------------------------------------------------
//!	@file	Block.h
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
class ComponentLiftable;
USING_PTR(CandyBomb);

class CandyBomb : public Object
{
public:
    BP_OBJECT_DECL(CandyBomb, u8"プレイシーンのキャンディー爆弾オブジェクト")

    //@
    bool Init() override;    //!< 初期化

    //当たり判定のコールバック関数
    //! @param hit_info [in] ヒットした相手側の情報
    void OnHit(const ComponentCollision::HitInfo& hit_info) override;

private:
    std::weak_ptr<ComponentLiftable> liftable_component_;    //持ち上げコンポーネントの弱参照

    const float RADUIS_ = 2.0f;

    const float SCALE_ = 0.3f;

    const float DISTANCE_RANGE_ = 25.0f;

    bool has_been_lifted = false;    //一度でも持ち上げられたことがあるかを保持する関数
};
