#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(StateBase);

class StateBase : public Component
{
public:
    BP_COMPONENT_DECL(StateBase, u8"StateBase");

    void Init() override;

    void Update() override;

    void GUI() override;

    //状態を変化させる関数
    //! @tparam [in] class T ステートタイプ
    template <class T>
    void ChangeState();

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

//---------------------------------------------------------------------------
//! 状態を変化させる関数
//---------------------------------------------------------------------------
template <class T>
void StateBase::ChangeState()
{
    RemoveThisComponent();
    auto owner = GetOwner();
    owner->AddComponent<T>();
}

CEREAL_CLASS_VERSION(StateBase, 1);
