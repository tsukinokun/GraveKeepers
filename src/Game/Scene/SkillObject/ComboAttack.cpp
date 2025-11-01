//---------------------------------------------------------------------------
//!	@file	ComboAttack.cpp
//! @brief	ゲームシーンの連撃オブジェクト
//! @auther 吉田
//---------------------------------------------------------------------------
#include "ComboAttack.h"
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentStatus.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ComboAttack::Init()
{
    __super::Init();

    SetName(u8"連撃");
    SetEffect(u8"data/PoyPoy/Effect/ComboAttack/ComboAttack.efkefc");
    //---------------------------------------------------------------------------------
    // コリジョンを追加する
    //---------------------------------------------------------------------------------
    auto col_comp = AddComponent<ComponentCollisionSphere>();
    col_comp->SetRadius(8.0f);
    col_comp->SetCollisionGroup(ComponentCollision::CollisionGroup::SKILL);                          // 所属するグループを「SKILL」とします
    col_comp->SetHitCollisionGroup(static_cast<u32>(ComponentCollision::CollisionGroup::PLAYER));    //PLAYERにのみ当たるように設定
    col_comp->SetOverlapCollisionGroup(0xffffffff);    //すべてオーバーラップ(押し戻さないが、OnHitにコールバックは行うように)するように設定
    collision_component_ = col_comp;
    return true;
}

//---------------------------------------------------------------------------
//! @brief	当たった情報はコールバックで送られてくる
//---------------------------------------------------------------------------
void ComboAttack::OnHit(const ComponentCollision::HitInfo& hit_info)
{
    //キャラクターが当たったらダメージを受ける
    if(auto other_owner = hit_info.hit_collision_->GetOwner()) {
        //当たったオブジェクトの名前が使用者か確認
        if(other_owner->GetName() == skill_owner_name_) {
            //使用者なので何もしない
            return;
        }
        //ステータスコンポーネントを取得
        if(auto status_comp = other_owner->GetComponent<ComponentStatus>()) {
            status_comp->GetInvincibilityTimer(0.5f);
            //ダメージを与える(とりあえず5)
            status_comp->TakeDamage(ATTACK_DAMAGE_);
        }
    }
}
