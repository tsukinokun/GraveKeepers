//---------------------------------------------------------------------------
//!	@file	SkillObjectBase.cpp
//! @brief	スキルのオブジェクトの基底クラス
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "SkillObjectBase.h"
#include <System/Component/ComponentEffect.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool SkillObjectBase::Init()
{
    __super::Init();

    SetName(u8"Skill");

    //---------------------------------------------------------------------------------
    // エフェクトコンポーネント
    //---------------------------------------------------------------------------------
    effect_component_ = AddComponent<ComponentEffect>();

    //---------------------------------------------------------------------------------
    // 更新処理の登録
    //---------------------------------------------------------------------------------
    {
        auto update = [this]() {
            if(auto eff = effect_component_.lock()) {
                if(eff->IsPlaying())
                    return;
            }

            // Effect終了で自分ごと消去
            Scene::Object::Release(SharedThis());
        };
        SetProc("update", update, ProcTiming::Update, ProcPriority::NORMAL);
    }
    return true;
}

//---------------------------------------------------------------------------
// エフェクトを設定
//! @param effect_name エフェクト名
//! @return 自分自身のshared_ptr
//---------------------------------------------------------------------------
std::shared_ptr<SkillObjectBase> SkillObjectBase::SetEffect(const std::string_view effect_name)
{
    if(auto eff = effect_component_.lock()) {
        eff->Load(effect_name);
        eff->Play();    // ロードと同時にエフェクトを再生
    }
    return dynamic_pointer_cast<SkillObjectBase>(shared_from_this());
}

//---------------------------------------------------------------------------
// スキル使用者の名前を設定する
//---------------------------------------------------------------------------
std::shared_ptr<SkillObjectBase> SkillObjectBase::SetSkillOwnerName(const std::string_view skill_owner_name)
{
    skill_owner_name_ = skill_owner_name;
    return dynamic_pointer_cast<SkillObjectBase>(shared_from_this());
}
