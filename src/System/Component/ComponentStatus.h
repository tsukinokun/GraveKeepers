#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentStatus);

//! @brief ジャンプ機能コンポーネント
class ComponentStatus : public Component
{
public:
    BP_COMPONENT_DECL(ComponentStatus, u8"ステータスコンポーネント");
    //! @{
    // 初期化処理
    void Init() override;

    // 更新処理
    void Update() override;

    // ImGui
    void GUI() override;

    //HPのゲッタ
    //! @retval HP
    int GetHitPoints() const;

    //HPのセッタ
    //! @param hp [in] ヒットポイント
    //! @retval 自身のポインタ
    std::shared_ptr<ComponentStatus> SetHitPoints(int hp);

    //ダメージを受ける関数
    //! @param damage [in] ダメージ量
    //! @retval 自身のポインタ
    //! @attention 無敵時間中の場合は自動的にダメージを受けないようになっています。
    std::shared_ptr<ComponentStatus> TakeDamage(int damage);

    //HPが0であることを返す関数
    //! @retval HPが0か否か
    bool IsDead() const;

    //移動スピードのセッタ
    //! @param speed [in] 移動スピード
    //! @retval 自身のポインタ
    std::shared_ptr<ComponentStatus> SetSpeed(float move_speed);

    //移動スピードのゲッタ
    //! @retval 移動スピード
    float GetSpeed() const;
    //! @}

    void SetInvincibilityTime(float time);

private:
    //デルタタイム
    std::chrono::time_point<std::chrono::steady_clock> prev_time_    = std::chrono::high_resolution_clock::now();    //前フレームの時間
    std::chrono::time_point<std::chrono::steady_clock> current_time_ = std::chrono::high_resolution_clock::now();    //現在の時間
    //HP関連
    int   hp_                  = 0;       //ヒットポイント
    float INVINCIBILITY_TIME_  = 2.0f;    //無敵時間
    float invincibility_timer_ = 0.0f;    //無敵時間タイマー
    //スピード関連
    float move_speed_ = 0.2f;    //移動スピード

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentStatus, 3);
