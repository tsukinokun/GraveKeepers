//---------------------------------------------------------------------------
//!	@file	ComponentComboAttack.cpp
//! @brief	連撃のコンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentComboAttack.h"
#include <System/Component/ComponentEffect.h>
#include <Game/Scene/SkillObject/ComboAttack.h>
#include <System/Component/ComponentStatus.h>

//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentComboAttack::Init()
{    // 初期化処理
    __super::Init();
}

//---------------------------------------------------------------------------
//! @brief	更新処理関数
//---------------------------------------------------------------------------
void ComponentComboAttack::Update()
{    // 初期化処理
    __super::Update();
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentComboAttack::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"連撃コンポーネント")) {
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
std::shared_ptr<ComponentSkill> ComponentComboAttack::UseSkill()
{
    __super::UseSkill();
    auto owner = GetOwner();

    //---------------------------------------------------------------------------
    // スキルオブジェクトの生成
    //---------------------------------------------------------------------------
    auto comboattack = Scene::Object::Create<ComboAttack>();    //ポイズンオブジェクトを生成
    comboattack->SetSkillOwnerName(owner->GetName());           //スキル使用者の名前を

    //高さの値は仮、後に持ち上げているオブジェクトの位置に変更するはず。
    const float3 pos = owner->GetTranslate() + float3(0.0f, 20.0f, 0.0f);
    comboattack->SetTranslate(pos);

    //プレイヤーの向きに合わせる
    comboattack->SetRotationAxisXYZ(owner_->GetRotationAxisXYZ());
    //自身のポインタを変換
    return dynamic_pointer_cast<ComponentSkill>(shared_from_this());
}

CEREAL_REGISTER_TYPE(ComponentComboAttack)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentComboAttack)
