//---------------------------------------------------------------------------
//!	@file	CandyBomb.h
//! @brief	プレイシーンのキャンディー爆弾オブジェクト
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
//前方宣言
class ComponentLiftable;
class ComponentCollisionSphere;
USING_PTR(CandyBomb);

class CandyBomb : public Object
{
public:
    BP_OBJECT_DECL(CandyBomb, u8"プレイシーンのキャンディー爆弾オブジェクト")

    //@
    bool Init() override;    //!< 初期化

    void Update() override;    //!< 更新

    //当たり判定のコールバック関数
    //! @param hit_info [in] ヒットした相手側の情報
    void OnHit(const ComponentCollision::HitInfo& hit_info) override;

private:
    std::weak_ptr<ComponentLiftable>        liftable_component_;                                        //持ち上げコンポーネントの弱参照
    std::weak_ptr<ComponentCollisionSphere> collision_component_;                                       //コリジョンコンポーネントの弱参照
    const float                             RADUIS_                = 2.0f;                              //コリジョンの半径
    const float                             SCALE_                 = 0.3f;                              //サイズ
    const float                             DISTANCE_RANGE_X       = 40.0f;                             //X方向の設置範囲
    const float                             DISTANCE_RANGE_Z_MINUS = 25.5f;                             //Z方向の設置範囲(かぼちゃ方面)
    const float                             DISTANCE_RANGE_Z_PLUS  = DISTANCE_RANGE_Z_MINUS + 10.0f;    //Z方向の設置範囲(かぼちゃ方面)

    const float POP_HEIGHT_MIN_ = -9.0f;    //生成時の最低高さ
    const float POP_HEIGHT_MAX_ = -3.0f;    //生成時の最高高さ

    bool has_been_lifted_ = false;    //一度でも持ち上げられたことがあるかを保持する関数
    //上昇する
    bool is_rising_ = true;
};
