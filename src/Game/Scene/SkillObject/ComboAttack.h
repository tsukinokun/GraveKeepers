//---------------------------------------------------------------------------
//!	@file	ComboAttack.h
//! @brief	連撃スキルのオブジェクト
//! @auther 吉田
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "SkillObjectBase.h"

//前方宣言
class ComponentCollisionSphere;
USING_PTR(ComboAttack);

class ComboAttack : public SkillObjectBase
{
public:
    BP_OBJECT_DECL(ComboAttack, u8"プレイシーンの連撃スキルのオブジェクト")

    //@
    bool Init() override;    //!< 初期化
    //---------------------------------------------------------------------------
    //	当たった情報はコールバックで送られてくる
    //! @param	hit_info 当たった情報
    //---------------------------------------------------------------------------
    void OnHit(const ComponentCollision::HitInfo& hit_info) override;

private:
    std::weak_ptr<ComponentCollisionSphere> collision_component_;    //!< 衝突判定用カプセルコンポーネント
    const int                               ATTACK_DAMAGE_ = 15;     //!< 攻撃力
};
