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

    //速度を設定する
    //! @param velocity [in] 設定する速度
    void SetVelocity(float3 velocity);

    //速度を取得する
    //! @retval 速度
    float3 GetVelocity() const;

    //質量を取得する
    //! @retval 質量
    float GetMass() const;

    //反発係数を設定する
    //! @param restitution [in] 反発係数の値
    void SetRestitution(float restitution);

    //反発係数の取得
    //! @retval 反発係数の値
    float GetRestitution() const;

    //質量を設定する
    //! @param mass [in] 質量の値
    void SetMass(float mass);

    //! @}

private:
    std::chrono::time_point<std::chrono::steady_clock> prev_time_    = std::chrono::high_resolution_clock::now();
    std::chrono::time_point<std::chrono::steady_clock> current_time_ = std::chrono::high_resolution_clock::now();

    float         mass_ = 15.0f;                   // 質量
    static float3 gravity_;                        // 重力加速度
    float3        acceleration_ = float3(0.0f);    // 加速度
    float3        velocity_     = float3(0.0f);    // 速度
    bool          use_gravity_  = true;            // 重力を使用するか
    float         drag_         = 1.0f;            // 空気抵抗係数
    float         restitution_  = 0.5f;            // 反発係数

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
