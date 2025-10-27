//---------------------------------------------------------------------------
//!	@file	ComponentPoison.cpp
//! @brief	ポイズンのコンポーネント
//! @auther 吉田
//---------------------------------------------------------------------------
#include "ComponentPoison.h"
#include <Game/Scene/SkillObject/Poison.h>
#include <System/Component/ComponentEffect.h>

//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentPoison::Init()
{    // 初期化処理
    __super::Init();
}

//---------------------------------------------------------------------------
//! @brief	更新処理関数
//---------------------------------------------------------------------------
void ComponentPoison::Update()
{    // 初期化処理
    __super::Update();
    //スキル使用条件を満たしたら
    if(is_use_skill_()) {
        UseSkill();    // スキルを使用する
    }
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentPoison::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"ポイズンコンポーネント")) {
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

//---------------------------------------------------------------------------
//! @brief	スキルを発動する
//---------------------------------------------------------------------------
std::shared_ptr<ComponentSkill> ComponentPoison::UseSkill()
{
    __super::UseSkill();
    auto owner = GetOwner();
    //---------------------------------------------------------------------------
    // スキルオブジェクトの生成
    //---------------------------------------------------------------------------
    auto poison = Scene::Object::Create<Poison>();    //ポイズンオブジェクトを生成
    poison->SetSkillOwnerName(owner->GetName());      //スキル使用者の名前を設定(スキルが使用者に干渉しなくするために設定不可欠。)
    //高さの値は仮、後に持ち上げているオブジェクトの位置に変更するはず。
    const float3 pos = owner->GetTranslate() + float3(0.0f, 1.0f, 0.0f);
    poison->SetTranslate(pos);

    poison->SetScaleAxisXYZ(SCALE_);
    poison->GetComponent<ComponentEffect>()->SetPlaySpeed(0.2f);
    //自身のポインタを変換
    return dynamic_pointer_cast<ComponentSkill>(shared_from_this());
}

CEREAL_REGISTER_TYPE(ComponentPoison)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentPoison)
