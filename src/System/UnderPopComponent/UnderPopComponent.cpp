#include "UnderPopComponent.h"
#include <System/RandomRange/RandomRange.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentRigidbody.h>

void UnderPopComponent::Init()
{
    Super::Init();

    auto owner = GetOwner();

    owner->SetTranslate(float3(GetRandomRangeF(-DISTANCE_RANGE_X, DISTANCE_RANGE_X), -5.0f, GetRandomRangeF(-DISTANCE_RANGE_Z_MINUS, DISTANCE_RANGE_Z_PLUS)));
}

void UnderPopComponent::Update()
{
    Super::Update();

    auto owner     = GetOwner();
    auto translate = owner->GetTranslate();
    if(translate.y < 0.0f) {
        translate.y += 0.1f;
        owner->SetTranslate(translate);
    }
    else {
        if(is_under_ == false) {
            is_under_ = true;
            //---------------------------------------------------------------------------------
            //球のコリジョンをアタッチ（モデルの形通りにするとすり抜けたため形通りにはしていません）
            //---------------------------------------------------------------------------------
            owner->AddComponent<ComponentCollisionSphere>();

            auto col = owner->GetComponent<ComponentCollisionSphere>();
            // コリジョンの半径を設定
            col->SetRadius(RADUIS_);

            col->SetCollisionGroup(ComponentCollision::CollisionGroup::ITEM);    //当たり判定グループを設定

            owner->AddComponent<ComponentRigidbody>();
        }
    }
}
