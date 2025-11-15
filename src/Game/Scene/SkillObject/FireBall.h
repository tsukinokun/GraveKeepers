//---------------------------------------------------------------------------
//!	@file	FireBall.h
//! @brief	火球スキルのオブジェクト
//! @auther 田中南々子
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "SkillObjectBase.h"
//前方宣言
class ComponentCollisionSphere;
USING_PTR(FireBall);

class FireBall : public SkillObjectBase
{
public:
    BP_OBJECT_DECL(FireBall, u8"プレイシーンの火球スキルのオブジェクト")

    //@
    bool Init() override;    //!< 初期化

    void Update() override;    //!< 更新

    void SetDirection(float3 direction);    //!< 進行方向を設定

    //---------------------------------------------------------------------------
    //	当たった情報はコールバックで送られてくる
    //! @param	hit_info 当たった情報
    //---------------------------------------------------------------------------
    void OnHit(const ComponentCollision::HitInfo& hit_info) override;

private:
    std::weak_ptr<ComponentCollisionSphere> collision_component_;    //!< 衝突判定用カプセルコンポーネント
    const int                               ATTACK_DAMAGE_ = 10;     //!< 攻撃力
    const float                             RADUIS_        = 2.0f;

    bool  is_hit_ = false;    //!< 当たったかどうかのフラグ
    float radius_ = 0.0f;     //!< 半径拡大用タイマー

    bool effect_changed_ = false;    //!< エフェクト変更フラグ

    float throw_virtical_power_   = -50.0f;    //垂直方向に投げる力
    float throw_horizontal_power_ = 50.0f;     //水平方向に投げる力

    float3 direction_ = 0.0f;    //!< 進行方向

    float3 hit_pos_ = 0.0f;    //!< 当たった位置
};
