#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentHp);

//! @brief ジャンプ機能コンポーネント
class ComponentHp : public Component
{
public:
    BP_COMPONENT_DECL(ComponentHp, u8"HPコンポーネント");
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
    void SetHitPoints(int hp);

    //ダメージを受ける関数
    //! @param damage [in] ダメージ量
    void TakeDamage(int damage);
    //! @}

private:
    int hp_ = 0;    //ヒットポイント

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentHp, 3);
