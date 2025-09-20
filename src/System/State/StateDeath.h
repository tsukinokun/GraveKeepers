#pragma once
#include <System/Scene.h>
#include "StateBase.h"

USING_PTR(StateDeath);

class StateDeath : public StateBase
{
public:
    BP_COMPONENT_DECL(StateDeath, u8"StateDeath");

    void Init() override;

    void Update() override;
    void GUI() override;

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

CEREAL_CLASS_VERSION(StateDeath, 1);
