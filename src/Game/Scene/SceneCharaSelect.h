//---------------------------------------------------------------------------
//!	@file	SceneCharaSelect.h
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>

//===========================================================================
//! アニメーションサンプルシーン
//===========================================================================
class SceneCharaSelect : public Scene::Base
{
public:
    BP_CLASS_DECL(SceneCharaSelect, u8"キャラセレクトのシーン")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

private:
    int                                selected_character_index_ = 0;        //!< 選択されたキャラクターのインデックス、はじめは0から
    float                              rad_offset_               = 90.0f;    //!< キャラクター配置の角度の実際のオフセット
    float                              rad_display_offset_       = 90.0f;    //!< 画面に表示されるキャラクター配置の角度のオフセット
    std::vector<std::weak_ptr<Object>> manage_characters_;                   //!< 管理しているキャラクターオブジェクトの配列
};
