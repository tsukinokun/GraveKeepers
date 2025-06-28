#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentRigidbody);

//! 剛体コンポーネント
class ComponentRigidbody : public Component
{
public:
    BP_COMPONENT_DECL(ComponentRigidbody, u8"剛体コンポーネント");
    //! @{
    //	初期化処理
    void Init() override;

    //	物理演算
    void PrePhysics() override;

    //ImGui
    void GUI() override;

    //衝撃を与える
    //! @param impulse [in]	与える衝撃の大きさ
    void AddImpulse(const float3& impulse);    //衝撃を与える

    //! @}

private:
    float3 velocity_      = float3(0.0f, 0.0f, 0.0f);    //速度
    float  damping_ratio_ = 0.9f;                        //減衰率、この値で毎フレームvelocity_にスケールをすることで速度を減衰させる。

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentRigidbody, 3);
