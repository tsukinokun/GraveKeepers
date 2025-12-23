//---------------------------------------------------------------------------
//!	@file	Player.h
//! @brief	プレイヤーヘッダー
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>

USING_PTR(Player);
class Character;

class Player : public Object
{
public:
    BP_OBJECT_DECL(Player, u8"プレイシーンのプレイヤー")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

    void OnHit(const ComponentCollision::HitInfo& hit_info) override;

    //---------------------------------------------------------------------------
    // コントロールしているキャラクターを取得
    //! @return キャラクターのweak_ptr
    //---------------------------------------------------------------------------
    std::weak_ptr<Character> GetControllCharacter() const;

private:
    std::weak_ptr<Character> controll_character_;

    int selected_skill_index_ = 0;

    enum
    {
        SKILL_FIREBALL = 0,
        SKILL_DASH,
        SKILL_POISON,
        SKILL_COMBO_ATTACK,
    };
};
