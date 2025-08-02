#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentSkill);

class ComponentSkill : public Component
{
public:
    BP_COMPONENT_DECL(ComponentSkill, u8"スキルコンポーネント");
    //! @{
    //	初期化処理
    void Init() override;

    //更新処理
    void Update() override;

    //ImGui
    void GUI() override;

    //スキルを使用する条件を設定する
    //! @param is_use_skill [in] スキルを使用する条件
    //! @retval 自身のポインタ
    std::shared_ptr<ComponentSkill> SetConditionsForUseSkill(const std::function<bool()>& is_use_skill);

    //スキルを発動する
    //! @retval 自分のポインタ
    virtual std::shared_ptr<ComponentSkill> UseSkill();
    //! @}

protected:
    std::function<bool()> is_use_skill_;    //これがtrueならスキルを使用する

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentSkill, 3);
