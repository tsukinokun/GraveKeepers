//---------------------------------------------------------------------------
//!	@file	CandyBomb.cpp
//! @brief	ゲームシーンのキャンディ爆弾オブジェクト
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

    //---------------------------------------------------------------------------------
    //球のコリジョンをアタッチ（モデルの形通りにするとすり抜けたため形通りにはしていません）
    //---------------------------------------------------------------------------------
    auto candy_col = AddComponent<ComponentCollisionSphere>();
    //block_col->UseGravity();
    // コリジョンの半径を設定
    candy_col->SetRadius(RADUIS_);
    // 当たり判定グループを設定
    candy_col->SetCollisionGroup(ComponentCollision::CollisionGroup::ITEM);
    collision_component_ = candy_col;

    //---------------------------------------------------------------------------------
    //	モデルコンポーネント
    //---------------------------------------------------------------------------------
    AddComponent<ComponentModel>("data/PoyPoy/Model/Object/CandyBomb/candy.mv1");

    auto model = GetComponent<ComponentModel>();
    model->SetScaleAxisXYZ(SCALE_);    //サイズの設定
    //位置の設定（-DISTANCE_RANGE_からDISTANCE_RANGE_の間に設置）
    SetTranslate(float3(GetRandomRangeF(-DISTANCE_RANGE_, DISTANCE_RANGE_), 0.0f, GetRandomRangeF(-DISTANCE_RANGE_, DISTANCE_RANGE_)));

    //---------------------------------------------------------------------------------
    //	剛体コンポーネント
    //---------------------------------------------------------------------------------
    auto rb = AddComponent<ComponentRigidbody>();
    //---------------------------------------------------------------------------------
    //	持ち上げられ機能コンポーネント
    //---------------------------------------------------------------------------------
    liftable_component_ = AddComponent<ComponentLiftable>();
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
        //モデルがあれば削除
        if(auto model = GetComponent<ComponentModel>()) {
            RemoveComponent<ComponentModel>();
            //モデルを消したタイミングでエフェクトを生成
            const std::string eff_name = "data/PoyPoy/Effect/CandyBomb/Simple_Sprite_BillBoard.efkefc";
            const float3      pos      = GetTranslate();
            auto              effect   = ComponentEffect::Object::Create(eff_name, pos);
        }
        else {
            //なければ自身を削除(1フレームだけ爆発判定をつける)
            Scene::Object::Release(shared_from_this());
        }

        //コリジョンを膨らませる
        if(auto collision = collision_component_.lock()) {
            collision->SetRadius(RADUIS_ * 7.0f);
        }

        //ヒットした相手方の剛体に力を加える
        if(auto other = hit_info.hit_collision_->GetOwner()) {
            if(auto other_rb = other->GetComponent<ComponentRigidbody>()) {
                //自分から相手への方向ベクトルを計算
                float3 direction = other->GetTranslate() - GetTranslate();
                direction        = normalize(direction);
                //力を加える
                const float force_strength = 400.0f;
                other_rb->AddImpulse(direction * force_strength);
            }
        }
    }
}
