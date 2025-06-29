#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentLiftable);

//! @持ち上げ機能コンポーネント
class ComponentLiftable : public Component
{
public:
    BP_COMPONENT_DECL(ComponentLiftable, u8"持ち上げコンポーネント");
    //! @{
    // 初期化処理
    void Init() override;

    // 更新処理
    void Update() override;

    //ImGui
    void GUI() override;

    //持ち上げられているかの状態を変更
    void SetLiftedFlag(bool lifted_flag);

    //持ち上げられているか否かを返す関数
    bool IsLifted();
    //! @}

private:
    bool is_lifted_ = false;    //持ち上げられているか否かを保持する変数

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
