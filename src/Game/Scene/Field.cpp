//---------------------------------------------------------------------------
//!	@file	Field.cpp
//! @brief	フィールド
//---------------------------------------------------------------------------
#include "Field.h"
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionModel.h>
#include <System/Component/ComponentLiftable.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Field::Init()
{
    __super::Init();
    SetName(u8"Field");
    //フィールドの読み込み
    auto model_comp = AddComponent<ComponentModel>("data/PoyPoy/Model/Stage/stage.mv1");
    AddComponent<ComponentCollisionModel>()->AttachToModel();
    //座標の設定
    SetTranslate({0, -1, 0});
    //回転
    SetRotationAxisXYZ({0, 180, 0});
    //モデルを１０倍している
    SetScaleAxisXYZ(1.5f);
    auto sky2_comp = AddComponent<ComponentModel>("data/PoyPoy/Model/Stage/Background.mv1");
    //座標を後ろに
    sky2_comp->SetTranslate({0, -400, 500});
    //スケールの調整
    sky2_comp->SetScaleAxisXYZ(0.8f);

    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Field::Update()
{
    __super::Update();
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void Field::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void Field::Exit()
{
    __super::Exit();
}

//---------------------------------------------------------------------------------
////! GUI表示
//---------------------------------------------------------------------------------
void Field::GUI()
{
    __super::GUI();
}

//---------------------------------------------------------------------------------
//! ヒット時処理
//---------------------------------------------------------------------------------
void Field::OnHit(const ComponentCollision::HitInfo& hit_info)
{
    __super::OnHit(hit_info);
    auto hit_owner = hit_info.hit_collision_->GetOwner();
    if(auto hit_comp_liftable = hit_owner->GetComponent<ComponentLiftable>()) {
        hit_comp_liftable->SetLiftedFlag(false);
    }
}
