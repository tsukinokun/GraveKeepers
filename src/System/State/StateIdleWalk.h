#pragma once
#include <System/Scene.h>
#include "StateBase.h"

USING_PTR(StateIdleWalk);
class ComponentModel;    //前方宣言

class StateIdleWalk : public StateBase
{
public:
    BP_COMPONENT_DECL(StateIdleWalk, u8"StateIdleWalk");

    void Init() override;

    void Update() override;
    void GUI() override;

private:
    //--------------------------------------------------------------------
    // 通常時の待機モーションと歩行モーションの切り替え処理
    //--------------------------------------------------------------------
    void IdleWalkChange(std::shared_ptr<ComponentModel> model, float3 diff);

private:
    float3 prev_pos_ = float3(0.0f, 0.0f, 0.0f);    //前フレームの座標

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(StateIdleWalk, 1);
