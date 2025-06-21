#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentLift);

class ComponentLift : public Component
{
public:
    BP_COMPONENT_DECL(ComponentLift, u8"持ち上げコンポーネント");

    void Init() override;

    void Update() override;

    void GUI() override;

    void SetConditionsForLifting(std::function<bool()> conditions);    //ラムダ式で持ち上げ条件を記述
    void SetConditionsForThrow(std::function<bool()> conditions);      //ラムダ式で投げ条件を記述

private:
    std::function<bool()> conditions_for_lifting_;    //持ち上げ条件を記述する関数を入れる

    std::function<bool()> conditions_for_throw_;    //投げる条件を記述する

    std::weak_ptr<Object> lift_object_;    //持ち上げているオブジェクト

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentLift, 3);
