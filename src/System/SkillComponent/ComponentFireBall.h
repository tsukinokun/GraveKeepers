#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentFireBall);

class ComponentFireBall : public Component
{
public:
    BP_COMPONENT_DECL(ComponentFireBall, u8"ファイアボールのコンポーネント");
    //! @{
    //	初期化処理
    void Init() override;

    //	物理演算
    void LateDraw() override;

    //ImGui
    void GUI() override;
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

CEREAL_CLASS_VERSION(ComponentFireBall, 3);
