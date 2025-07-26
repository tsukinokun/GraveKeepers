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
class ComponentHp;
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

protected:
    std::weak_ptr<ComponentLift>             lift_component_;         //持ち上げcomponent
    std::weak_ptr<ComponentLiftable>         liftable_component_;     // 持ち上げられcomponent
    std::weak_ptr<ComponentJump>             jump_component_;         // ジャンプcomponent
    std::weak_ptr<ComponentModel>            model_component_;        // モデルcomponent
    std::weak_ptr<ComponentCollisionCapsule> collision_component_;    //コリジョンcomponent
    std::weak_ptr<ComponentHp>               hp_component_;           // ヒットポイントcomponent
    std::weak_ptr<ComponentRigidbody>        rigidbody_component_;    // 剛体component

private:
    const float RADIUS_              = 4.0f;           //半径
    const float TOP_POINT_           = RADIUS_ * 3;    //しゃがんでいない頭の位置
    const float SQUAT_TOP_POINT_     = RADIUS_ * 2;    //しゃがんでいるときの頭の位置
    const float FACE_DOWN_TOP_POINT_ = RADIUS_;        //うつ伏せの時の頭の位置
                                                       //const int	HP_MAX_				 = 200;			   //HPの最大値

    float neutralpos_   = TOP_POINT_;    //円の位置の高さ
    bool  is_face_down_ = false;         //うつ伏せの状態かを確認する用の変数
};
