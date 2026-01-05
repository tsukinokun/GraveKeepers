//---------------------------------------------------------------------------
//!	@file	Enemy.h
//! @brief	エネミーヘッダー
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <Game/Scene/UIObject/UIText.h>

class Character;
USING_PTR(Enemy);

class Enemy : public Object
{
public:
    BP_OBJECT_DECL(Enemy, u8"プレイシーンのエネミー")

    //@}
    bool Init() override;    //!< 初期化

    //---------------------------------------------------------------------------
    // コントロールしているキャラクターを取得
    //! @return キャラクターのweak_ptr
    //---------------------------------------------------------------------------
    std::weak_ptr<Character> GetControllCharacter() const;    //コントロールしているキャラクターを取得

    void SetDesiredCharacterName(const std::string& name) { desired_character_name_ = name; }

    void SetDisplayName(const std::string& name);

private:
    std::weak_ptr<Character> controll_character_;    //コントロールしているキャラクター

    std::string desired_character_name_;    // 生成時に優先するキャラ名（空なら自動）

    std::shared_ptr<UIText> name_ui_;
    std::string             display_name_;
};
