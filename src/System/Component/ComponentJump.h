#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentJump);

//! @brief ジャンプ機能コンポーネント
class ComponentJump : public Component
{
public:
    BP_COMPONENT_DECL(ComponentJump, u8"ジャンプコンポーネント");
    //! @{
    // 初期化処理
    void Init() override;

    // 更新処理
    void Update() override;

    // ImGui
    void GUI() override;

    // ジャンプフレームをセット
    //! @param value [in] フレーム
    void SetJumpFrame(int value);

    // ジャンプ時の衝撃をセット
    //! @param value [in] y座標に飛び上がる高さ
    void SetJumpImpulse(float value);

    //有効無効をセット
    //! @param enable_flag [in] ジャンプが有効か無効か
    void SetEnable(bool enable_flag = true);

    //ジャンプをしているかを返す関数
    bool IsJumping();

    //外部からジャンプ条件を入れるための関数
    //! @param condition [in] 条件
    //! @attention これを設定しないと絶対にジャンプしないので注意
    void SetConditionsJump(std::function<bool()> condition);

    //! @}

private:
    std::function<bool()> conditions_jump_;    //ジャンプ条件

    int   jump_frame_max_   = 60;       //何フレームジャンプするか
    int   jump_frame_count_ = 0;        //ジャンプ中のフレーム
    float jump_force_       = 100.0;    //ジャンプで足す値
    float translate_hight_  = 0;        //実際にmatrixに入れる値

    bool set_enable_ = false;    //ジャンプをできるかどうかの変数

    bool is_jumping_ = false;    //ジャンプをしているかの変数

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentJump, 3);
