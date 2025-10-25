//---------------------------------------------------------------------------
//!	@file	Block.cpp
//! @brief	ゲームシーンのブロックオブジェクト
//---------------------------------------------------------------------------
#include "CandyBomb.h"
#include <System/RandomRange/RandomRange.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentRigidbody.h>
#include <System/Component/ComponentLiftable.h>
#include <System/Component/ComponentEffect.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool CandyBomb::Init()
{
    __super::Init();

    SetName(u8"キャンディー爆弾");

    //球のコリジョンをアタッチ（モデルの形通りにするとすり抜けたため形通りにはしていません）
    auto candy_col = AddComponent<ComponentCollisionSphere>();
    //block_col->UseGravity();
    // コリジョンの半径を設定
    candy_col->SetRadius(RADUIS_);

    AddComponent<ComponentModel>("data/PoyPoy/Model/Object/CandyBomb/candy.mv1");

    //モデルコンポーネントを取得
    auto obj = GetComponent<ComponentModel>();
    obj->SetScaleAxisXYZ(SCALE_);    //サイズの設定
    //位置の設定（-DISTANCE_RANGE_からDISTANCE_RANGE_の間に設置）
    SetTranslate(float3(GetRandomRangeF(-DISTANCE_RANGE_, DISTANCE_RANGE_), 0.0f, GetRandomRangeF(-DISTANCE_RANGE_, DISTANCE_RANGE_)));

    auto rb             = AddComponent<ComponentRigidbody>();
    liftable_component_ = AddComponent<ComponentLiftable>();    //持ち上げられ機能コンポーネント
    //--------------------------------------------------------------------
    // 更新処理で、一度でも持ち上げられたことがあるかを更新する
    //--------------------------------------------------------------------
    auto check_has_been_lifted = [this]() {
        if(auto liftable_comp = liftable_component_.lock()) {
            //持ち上げられていたら、一度でも持ち上げられていることにする。
            if(liftable_comp->IsLifted()) {
                has_been_lifted_ = true;
            }
        }
    };
    SetProc("check_has_been_lifted", check_has_been_lifted, ProcTiming::Update, ProcPriority::NONE);
    return true;
}

//---------------------------------------------------------------------------------
//!	ヒットした際に呼ばれるコールバック関数
//---------------------------------------------------------------------------------
void CandyBomb::OnHit(const ComponentCollision::HitInfo& hit_info)
{
    __super::OnHit(hit_info);
    //どこかにヒットしたタイミングで一度でも持ち上げられたことがあれば爆発
    if(has_been_lifted_) {
        Scene::Object::Release(shared_from_this());    //解放を行って
        //エフェクトを生成
        const std::string eff_name = "data/PoyPoy/Effect/CandyBomb/Simple_Sprite_BillBoard.efkefc";
        const float3      pos      = GetTranslate();
        auto              effect   = ComponentEffect::Object::Create(eff_name, pos);
    }
}
