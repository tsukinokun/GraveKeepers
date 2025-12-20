//---------------------------------------------------------------------------
//!	@file	Enemy.h
//! @brief	エネミーヘッダー
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>

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

private:
    std::weak_ptr<Character> controll_character_;    //コントロールしているキャラクター
};
