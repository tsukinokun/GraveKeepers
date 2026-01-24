//---------------------------------------------------------------------------
//!	@file	Character.h
//! @brief	キャラクターのベースクラス
//! @author	山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>

USING_PTR(Character);
//componentの前方宣言
class ComponentLift;
class ComponentLiftable;
class ComponentModel;
class ComponentJump;
class ComponentStatus;
class ComponentRigidbody;

class Character : public Object
{
public:
    BP_OBJECT_DECL(Character, u8"キャラクターのベースクラス")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

    void OnHit(const ComponentCollision::HitInfo& hit_info) override;

    //---------------------------------------------------------------------------
    // 生存しているかどうかを返す関数
    //! @retval 生存しているかどうか
    //---------------------------------------------------------------------------
    bool IsAlive() const;

    //---------------------------------------------------------------------------
    // 投げられたものでダメージを負ったかを返す関数
    //! @retval オブジェクト衝突でダメージを負ったかどうか
    //---------------------------------------------------------------------------
    bool IsDamagedByThrownObject() const;

protected:
    std::weak_ptr<ComponentLift>             lift_component_;         //持ち上げcomponent
    std::weak_ptr<ComponentLiftable>         liftable_component_;     // 持ち上げられcomponent
    std::weak_ptr<ComponentJump>             jump_component_;         // ジャンプcomponent
    std::weak_ptr<ComponentModel>            model_component_;        // モデルcomponent
    std::weak_ptr<ComponentCollisionCapsule> collision_component_;    //コリジョンcomponent
    std::weak_ptr<ComponentStatus>           status_component_;       // ヒットポイントcomponent
    std::weak_ptr<ComponentRigidbody>        rigidbody_component_;    // 剛体component

private:
    const float RADIUS_    = 4.0f;           //半径
    const float TOP_POINT_ = RADIUS_ * 3;    //しゃがんでいない頭の位置

    float neutralpos_ = TOP_POINT_;    //円の位置の高さ

    bool dead_ = false;    // 死んでいるかどうか

    bool damaged_by_thrown_object_ = false;    // 投げられたものでダメージを負ったか
};
