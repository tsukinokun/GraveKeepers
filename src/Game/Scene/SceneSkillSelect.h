//---------------------------------------------------------------------------
//!	@file	SceneSkillSelect.h
//! @brief	スキル選択のシーン
//! @author 田中南々子
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>

class SceneSkillSelect : public Scene::Base
{
public:
    BP_CLASS_DECL(SceneSkillSelect, u8"スキル選択画面")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

private:
    int                                selected_skill_index_ = 0;        //!< 選択されたスキルのインデックス、はじめは0から
    float                              rad_offset_           = 90.0f;    //!< スキル配置の角度の実際のオフセット
    float                              rad_display_offset_   = 90.0f;    //!< 画面に表示されるスキル配置の角度のオフセット
    std::vector<std::weak_ptr<Object>> manage_skills_;                   //!< 管理しているスキルオブジェクトの配列

    ObjectPtr fireball_     = nullptr;
    ObjectPtr dash_         = nullptr;
    ObjectPtr poison_       = nullptr;
    ObjectPtr combo_attack_ = nullptr;
};
