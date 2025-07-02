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
    //フィールドの読み込み（仮で山口先生の素材を入れている）
    auto com_comp = AddComponent<ComponentModel>("data/Sample/PoyPoy/Field/field.mv1");
    AddComponent<ComponentCollisionModel>()->AttachToModel();    //< GUIでの 「モデルにコリジョンを張り付ける」動作
    //座標の設定
    SetTranslate({0, 0, 0});
    //モデルを１０倍している
    SetScaleAxisXYZ(10.0f);
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
