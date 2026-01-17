//---------------------------------------------------------------------------
//!	@file	ComponentLiftable.h
//! @brief	持ち上げられ機能コンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>
class Character;
USING_PTR(ComponentLiftable);

//! @持ち上げ機能コンポーネント
class ComponentLiftable : public Component
{
public:
    BP_COMPONENT_DECL(ComponentLiftable, u8"持ち上げられ機能コンポーネント");
    //! @{
    // 初期化処理
    void Init() override;

    //ImGui
    void GUI() override;

    //----------------------------------------------------------------
    //持ち上げられているかの状態を変更
    //! @param lifted_flag 持ち上げられているか否かのフラグ
    //----------------------------------------------------------------
    void SetLiftedFlag(bool lifted_flag);

    //----------------------------------------------------------------
    //持ち上げられているか否かを返す関数
    //! @return 持ち上げられているか否か
    //----------------------------------------------------------------
    bool IsLifted();

    //----------------------------------------------------------------
    // 最後に持ち上げたキャラクターの弱参照を設定する関数
    //! @param character 持ち上げたキャラクターの弱参照
    //----------------------------------------------------------------
    void SetLiftCharacter(const std::weak_ptr<Character>& character);

    //----------------------------------------------------------------
    // 最後に持ち上げたキャラクターの参照を取得する関数
    //! @return 持ち上げたキャラクターのshared_ptr
    //----------------------------------------------------------------
    std::shared_ptr<Character> GetLiftCharacter();

    //--------------------------------------------------------------------
    // このオブジェクトが持ち上げ可能かを返す関数
    //! @return 持ち上げ可能か
    //--------------------------------------------------------------------
    bool CanBeLifted() const;

    //--------------------------------------------------------------------
    // 持ち上げ不可に変更する関数
    //--------------------------------------------------------------------
    void SetCannotBeLifted();

    void SetThrownFlag(bool flg) { is_thrown_ = flg; }

    bool IsThrown() const { return is_thrown_; }

    //! @}

private:
    std::weak_ptr<Character> lift_character_;           //オブジェクトを持ち上げたキャラクターの弱参照
    bool                     is_lifted_     = false;    //持ち上げられているか否かを保持する変数
    bool                     can_be_lifted_ = true;     //このオブジェクトが持ち上げ可能かどうか
    bool                     is_thrown_     = false;

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentLiftable, 3);
