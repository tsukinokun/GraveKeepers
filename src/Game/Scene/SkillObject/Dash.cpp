//---------------------------------------------------------------------------
//!	@file	Poison.cpp
//! @brief	ゲームシーンの突進オブジェクト
//---------------------------------------------------------------------------
#include "Dash.h"
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentStatus.h>
#include <System/Component/ComponentRigidbody.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Dash::Init()
{
    __super::Init();

    SetName(u8"突進");
    SetEffect(u8"data/PoyPoy/Effect/Dash/Simple_SpawnMethod1.efkefc");
    //---------------------------------------------------------------------------------
    // コリジョンを追加する
    //---------------------------------------------------------------------------------
    auto col_comp = AddComponent<ComponentCollisionSphere>();
    col_comp->SetRadius(3.0f);
    col_comp->SetCollisionGroup(ComponentCollision::CollisionGroup::SKILL);                          // 所属するグループを「SKILL」とします
    col_comp->SetHitCollisionGroup(static_cast<u32>(ComponentCollision::CollisionGroup::PLAYER));    //PLAYERにのみ当たるように設定
    col_comp->SetOverlapCollisionGroup(0xffffffff);    //すべてオーバーラップ(押し戻さないが、OnHitにコールバックは行うように)するように設定
    collision_component_ = col_comp;

    return true;
}

//---------------------------------------------------------------------------
//! @brief	当たった情報はコールバックで送られてくる
//---------------------------------------------------------------------------
void Dash::OnHit(const ComponentCollision::HitInfo& hit_info)
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
            //ダメージを与える(とりあえず20)
            status_comp->TakeDamage(ATTACK_DAMAGE_);
        }

        //ヒットした相手方の剛体に力を加える
        if(auto other = hit_info.hit_collision_->GetOwner()) {
            if(auto other_rb = other->GetComponent<ComponentRigidbody>()) {
                //自分から相手への方向ベクトルを計算
                float3 direction = other->GetTranslate() - GetTranslate();
                direction        = normalize(direction);
                //力を加える
                const float force_strength = 20.0f;
                other_rb->SetVelocity(float3(0.0f, 0.0f, 0.0f));    //速度を一旦リセット
                other_rb->AddImpulse(direction * force_strength);
            }
        }
    }
}
