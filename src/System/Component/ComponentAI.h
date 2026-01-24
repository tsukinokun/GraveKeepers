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
    std::weak_ptr<Character>                           target_object_;                                        //ターゲットオブジェクト
    float3                                             dir_         = float3(0.0f, 0.0f, 1.0f);               //!<内部の方向
    float3                                             display_dir_ = float3(0.0f, 0.0f, 1.0f);               //!< 表示方向
    std::weak_ptr<Object>                              target_block_;                                         //ターゲットブロック
    float                                              block_search_timeout_   = 0.0f;                        //ブロック探索のタイムアウト
    const float                                        BLOCK_SEARCH_TIME_      = 3.0f;                        //ブロック探索のタイムアウト時間(秒)
    const float                                        THROW_DISTANCE_         = 50.0f;                       //投げる際の適切な距離
    float3                                             prev_position_          = float3(0.0f, 0.0f, 0.0f);    //前フレームの位置
    float                                              stuck_time_             = 0.0f;                        //動けない時間のカウント
    const float                                        STUCK_TIME_THRESHOLD_   = 1.0f;                        //動けないと判定する時間(秒)
    const float                                        STUCK_DISTANCE_         = 0.5f;                        //動けないと判定する移動距離
    bool                                               jamp_signal_            = false;                       //ジャンプシグナル
    const float                                        TARGET_CHANGE_DISTANCE_ = 50.0f;                       //ターゲット切り替え距離

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
