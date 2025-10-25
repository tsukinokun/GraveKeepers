//---------------------------------------------------------------------------
//!	@file	Poison.cpp
//! @brief	ゲームシーンの毒オブジェクト
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "Poison.h"
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentStatus.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Poison::Init()
{
    __super::Init();

    SetName(u8"ポイズン");
    SetEffect(u8"data/PoyPoy/Effect/Poison/Poison.efkefc");
    //---------------------------------------------------------------------------------
    // コリジョンを追加する
    //---------------------------------------------------------------------------------
    auto col_comp = AddComponent<ComponentCollisionSphere>();
    col_comp->SetRadius(1.2f);
    col_comp->SetCollisionGroup(ComponentCollision::CollisionGroup::SKILL);                          // 所属するグループを「SKILL」とします
    col_comp->SetHitCollisionGroup(static_cast<u32>(ComponentCollision::CollisionGroup::PLAYER));    //PLAYERにのみ当たるように設定
    col_comp->SetOverlapCollisionGroup(0xffffffff);    //すべてオーバーラップ(押し戻さないが、OnHitにコールバックは行うように)するように設定
    collision_component_ = col_comp;

    return true;
}

//---------------------------------------------------------------------------
//! @brief	当たった情報はコールバックで送られてくる
//---------------------------------------------------------------------------
void Poison::OnHit(const ComponentCollision::HitInfo& hit_info)
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
            //ダメージを与える(とりあえず4)
            status_comp->TakeDamage(ATTACK_DAMAGE_);
        }
    }
}
