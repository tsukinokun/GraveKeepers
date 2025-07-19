#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentLift);

//! @持ち上げ機能コンポーネント
class ComponentLift : public Component
{
public:
    BP_COMPONENT_DECL(ComponentLift, u8"持ち上げコンポーネント");
    //! @{
    // 初期化処理
    void Init() override;

    // 更新処理
    void Update() override;

    //ImGui
    void GUI() override;

    //ラムダ式で持ち上げ条件を記述
    //! @param conditions [in] 持ち上げ条件
    void SetConditionsForLifting(std::function<bool()> conditions);

    //ラムダ式で投げ条件を記述
    //! @param conditions [in] 投げ条件
    void SetConditionsForThrow(std::function<bool()> conditions);

    //名前から、監視対象になるオブジェクトかどうかを返す
    //! @param name [in] チェックしたい名前
    //! @retval true 監視対象になる
    bool CheckLiftObjName(const std::string& name);

    //持ち上げ中か否かを返す関数
    //! @return 持ち上げ中か否か
    bool IsLifting();
    //! @}

private:
    std::function<bool()> conditions_for_lifting_;    //持ち上げ条件を記述する関数を入れる

    std::function<bool()> conditions_for_throw_;    //投げる条件を記述する

    std::weak_ptr<Object> lift_object_;    //持ち上げているオブジェクト

    const std::vector<std::string> IGNORE_NAMES_ = {"Camera", "Field", "UFO", "DebugCamera", "Wall"};    //チェックする名前

    float throw_virtical_power_   = -200.0f;    //垂直方向に投げる力
    float throw_horizontal_power_ = 300.0f;     //水平方向に投げる力

    float lift_angle_    = 40.0f;    //オブジェクトを持ち上げられる角度
    float lift_distance_ = 7.0f;     //オブジェクトを持ち上げることが可能な距離

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
