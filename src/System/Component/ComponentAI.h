#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentAI);

class Character;    // 前方宣言

//! @brief ジャンプ機能コンポーネント
class ComponentAI : public Component
{
public:
    BP_COMPONENT_DECL(ComponentAI, u8"AIコンポーネント");
    //! @{
    // 初期化処理
    void Init() override;

    // 更新処理
    void Update() override;

    // ImGui
    void GUI() override;

    //投げたい時にtrueを返す
    //! @retval 投げたいかどうか
    bool ThrowSignal();
    //! @}

private:
    float                                              lift_time_count_ = 0.0f;                                         //持ち上げている時間のカウント
    std::chrono::time_point<std::chrono::steady_clock> prev_time_       = std::chrono::high_resolution_clock::now();    //前フレームの時間
    std::chrono::time_point<std::chrono::steady_clock> current_time_    = std::chrono::high_resolution_clock::now();    //現在の時間
    float                                              move_speed_      = 0.2f;
    float                                              rot_speed_       = 5.0f;
    std::weak_ptr<Character>                           target_object_;                             //ターゲットオブジェクト
    float3                                             dir_         = float3(0.0f, 0.0f, 1.0f);    //!<内部の方向
    float3                                             display_dir_ = float3(0.0f, 0.0f, 1.0f);    //!< 表示方向

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentAI, 3);
