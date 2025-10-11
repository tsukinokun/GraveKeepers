#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>
#include "ComponentSkill.h"
USING_PTR(ComponentFireBall);

class ComponentPoison : public ComponentSkill
{
public:
    BP_COMPONENT_DECL(ComponentPoison, u8"ポイズンのコンポーネント");
    //! @{
    //	初期化処理
    void Init() override;

    //更新処理
    void Update() override;

    //ImGui
    void GUI() override;

    //毒設置
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

    const float SCALE_ = 6.0f;
};

CEREAL_CLASS_VERSION(ComponentPoison, 3);
