#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>
#include "ComponentSkill.h"
USING_PTR(ComponentComboAttack);

class ComponentComboAttack : public ComponentSkill
{
public:
    BP_COMPONENT_DECL(ComponentComboAttack, u8"連撃コンポーネント");
    //! @{
    //	初期化処理
    void Init() override;

    //更新処理
    void Update() override;

    //ImGui
    void GUI() override;

    //連撃を行う
    //! @retval 自分のポインタ
    std::shared_ptr<ComponentSkill> UseSkill() override;
    //! @}

private:
    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{
    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentComboAttack, 3);
