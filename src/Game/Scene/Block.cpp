//---------------------------------------------------------------------------
//!	@file	Block.cpp
//! @brief	ゲームシーンのブロックオブジェクト
//---------------------------------------------------------------------------
#include "Block.h"
#include <System/RandomRange/RandomRange.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentRigidbody.h>
#include <System/Component/ComponentLiftable.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Block::Init()
{
    __super::Init();

    SetName(u8"ブロック");
    SetStatus(Object::StatusBit::OnHitAllComponent, true);    //全てのコンポーネントに当たり判定を送る

    //球のコリジョンをアタッチ（モデルの形通りにするとすり抜けたため形通りにはしていません）
    //auto block_col = AddComponent<ComponentCollisionSphere>();
    //block_col->UseGravity();
    // コリジョンの半径を設定
    //block_col->SetRadius(RADUIS_);

    // ブロックの種類をランダムに決定
    auto rand = GetRand(BlockTypeMax - 1);

    switch(rand) {
    case CrossGrave:    //十字墓
        AddComponent<ComponentModel>("data/PoyPoy/Model/Object/CrossGrave/CrossGrave.mv1");
        break;

    case FlatGrave:    //平墓
        AddComponent<ComponentModel>("data/PoyPoy/Model/Object/FlatGrave/FlatGrave.mv1");

        break;
    default:    //十字墓
        AddComponent<ComponentModel>("data/PoyPoy/Model/Object/CrossGrave/CrossGrave.mv1");
        break;
    }

    //モデルコンポーネントを取得
    auto obj = GetComponent<ComponentModel>();
    obj->SetScaleAxisXYZ(SCALE_);    //サイズの設定
                                     //位置の設定（-DISTANCE_RANGE_からDISTANCE_RANGE_の間に設置）
    SetTranslate(float3(GetRandomRangeF(-DISTANCE_RANGE_, DISTANCE_RANGE_),
                        GetRandomRangeF(POP_HEIGHT_MIN_, POP_HEIGHT_MAX_),
                        GetRandomRangeF(-DISTANCE_RANGE_, DISTANCE_RANGE_)));

    auto rb = AddComponent<ComponentRigidbody>();
    rb->SetUseGravity(false);    //重力を使用しない

    AddComponent<ComponentLiftable>();    //持ち上げられ機能コンポーネント

    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Block::Update()
{
    __super::Update();

    auto pos = GetTranslate();
    //一定の高さまで上昇
    if(!is_rising_) {
        return;
    }
    pos.y += 0.1f;

    if(pos.y > 0.0f) {
        pos.y = 0.0f;
        //一定の高さまで行ったらストップ
        is_rising_ = false;
        //---------------------------------------------------------------------------------
        //球のコリジョンをアタッチ（モデルの形通りにするとすり抜けたため形通りにはしていません）
        //---------------------------------------------------------------------------------
        auto candy_col = AddComponent<ComponentCollisionCapsule>();
        //block_col->UseGravity();
        // コリジョンの半径を設定
        candy_col->SetRadius(RADUIS_);
        // コリジョンの高さを設定
        candy_col->SetHeight(HEIGHT_);
        // 当たり判定グループを設定
        candy_col->SetCollisionGroup(ComponentCollision::CollisionGroup::ITEM);

        auto rb = GetComponent<ComponentRigidbody>();
        rb->SetUseGravity(true);    //重力を使用する
    }
    SetTranslate(pos);

    //座標が一定以下になったら削除
    if(pos.y < -20.0f) {
        Scene::Object::Release(shared_from_this());
    }
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void Block::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void Block::Exit()
{
    __super::Exit();
}

//!GUI表示
void Block::GUI()
{
    __super::GUI();
}
