//---------------------------------------------------------------------------
//!	@file	Poison.cpp
//! @brief	ゲームシーンの突進オブジェクト
//---------------------------------------------------------------------------
#include "Dash.h"
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentStatus.h>
#include <System/Component/ComponentRigidbody.h>
#include <System/Component/ComponentEffect.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Dash::Init()
{
    __super::Init();

    SetName(u8"突進");
    // エフェクトコンポーネントを取得（既にある前提、なければAddComponent）
    auto effect = GetComponent<ComponentEffect>();
    if(effect) {
        // エフェクトファイルのロード
        effect->Load(u8"data/PoyPoy/Effect/Dash/Simple_SpawnMethod1.efkefc");

        // 再生開始
        effect->Play(true);
    }
    collision_component_.reset();
    return true;
}

//---------------------------------------------------------------------------
//! @brief	当たった情報はコールバックで送られてくる
//---------------------------------------------------------------------------
void Dash::OnHit(const ComponentCollision::HitInfo& hit_info)
{
}
