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

    //力を与える
    //! @param force [in]	与える力の大きさ
    void AddForce(const float3& force);    //力を与える

    //衝撃を与える
    //! @param impulse [in]	与える衝撃の大きさ
    void AddImpulse(const float3& impulse);    //衝撃を与える

    //重力を使用するかを変更
    //! @param use_gravity_flag [in] 重力を使用するかフラグ
    void SetUseGravity(bool use_gravity_flag);

    //速度を取得する
    //! @retval [in] 速度
    float3 GetVelocity() const;
    //! @}

private:
    float3 velocity_      = float3(0.0f, 0.0f, 0.0f);    //速度
    float  damping_ratio_ = 0.9f;                        //減衰率、この値で毎フレームvelocity_にスケールをすることで速度を減衰させる。

    std::chrono::time_point<std::chrono::steady_clock> prev_time_    = std::chrono::high_resolution_clock::now();    //前フレームの時間
    std::chrono::time_point<std::chrono::steady_clock> current_time_ = std::chrono::high_resolution_clock::now();    //現在の時間
    float                                              mass_         = 15.0f;                                        //質量
    static float3                                      gravity_;
    float3                                             acceleration_ = float3(0.0f, 0.0f, 0.0f);    //加速度
    float3                                             velocity_     = float3(0.0f, 0.0f, 0.0f);    //速度
    bool                                               use_gravity_  = true;                        //重力を使用するか否か
    float                                              drag_         = 15.0f;                       //空気抵抗
    float                                              angular_drag_ = 0.0f;                        //回転抵抗

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
