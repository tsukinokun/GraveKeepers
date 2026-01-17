//---------------------------------------------------------------------------
//!	@file	SkillFactory.h
//! @brief	スキルを生成するファクトリークラス
//! @author 田中南々子
//---------------------------------------------------------------------------
#include "SkillFactory.h"
#include <Game/Scene/SkillObject/FireBall.h>
#include <Game/Scene/SkillObject/Dash.h>
#include <Game/Scene/SkillObject/Poison.h>
#include <Game/Scene/SkillObject/ComboAttack.h>

//---------------------------------------------------------------------------
//! @brief SkillFactoryのインスタンスを取得する静的メソッド
//---------------------------------------------------------------------------
SkillFactory& SkillFactory::Instance()
{
    static SkillFactory instance;    // ここで一度だけ生成
    return instance;
}

//---------------------------------------------------------------------------
//! @brief スキルを生成するメソッド
//---------------------------------------------------------------------------
std::shared_ptr<SkillObjectBase> SkillFactory::CreateSkill(const std::string& skill_name)
{
    // 登録されているスキル名か確認
    auto it = skill_creators_.find(skill_name);
    if(it != skill_creators_.end()) {
        // 登録されていたら生成して返す
        return it->second();
    }
    // 登録されていなかったらnullptrを返す
    return nullptr;
}

//---------------------------------------------------------------------------
//! @brief 登録されているスキル名一覧を返す
//---------------------------------------------------------------------------
std::vector<std::string> SkillFactory::GetRegisteredSkillNames() const
{
    std::vector<std::string> skill_names;
    for(const auto& pair : skill_creators_) {
        skill_names.push_back(pair.first);
    }
    return skill_names;
}
