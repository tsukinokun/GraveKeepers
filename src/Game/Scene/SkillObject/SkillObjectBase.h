//---------------------------------------------------------------------------
//!	@file	SkillObjectBase.h
//! @brief	スキルのオブジェクトの基底クラス
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
//前方宣言
class ComponentEffect;
USING_PTR(SkillObjectBase);

class SkillObjectBase : public Object
{
public:
    BP_OBJECT_DECL(SkillObjectBase, u8"スキルのオブジェクトの基底クラス")

    //@
    bool Init() override;    //!< 初期化

    //---------------------------------------------------------------------------
    // エフェクトを設定
    //! @param effect_name エフェクト名
    //! @return 自分自身のshared_ptr
    //---------------------------------------------------------------------------
    std::shared_ptr<SkillObjectBase> SetEffect(const std::string_view effect_name);

    //---------------------------------------------------------------------------
    // スキル使用者の名前を設定する
    //! @param skill_owner_name スキル使用者の名前
    //! @return 自分自身のshared_ptr
    //---------------------------------------------------------------------------
    std::shared_ptr<SkillObjectBase> SetSkillOwnerName(const std::string_view skill_owner_name);

protected:
    std::weak_ptr<ComponentEffect> effect_component_;         //!< エフェクトコンポーネント
    std::string_view               skill_owner_name_ = "";    //!< スキル使用者の名前
};
