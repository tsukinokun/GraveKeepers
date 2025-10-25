#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>
#include "ComponentSkill.h"
USING_PTR(ComponentFireBall);

class ComponentFireBall : public ComponentSkill
{
public:
    BP_COMPONENT_DECL(ComponentFireBall, u8"ファイアボールのコンポーネント");
    //! @{
    //	初期化処理
    void Init() override;

    //更新処理
    void Update() override;

    //ImGui
    void GUI() override;

    //ファイアボールを発射する
    //! @retval 自分のポインタ
    std::shared_ptr<ComponentSkill> UseSkill() override;

private:
    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }

    //@}
    //コリジョンの移動用の関数
    void MoveCollision();

    float     THROW_VIRTICAL_POWER_   = 100.0f;    //垂直方向に投げる力
    float     THROW_HORIZONTAL_POWER_ = 300.0f;    //水平方向に投げる力
    ObjectPtr effect_;                             //エフェクト用のオブジェクト

    bool is_effect_play_ = false;    //エフェクトが再生されているかどうか

    const float COL_SIZE_MAX_     = 10.0f;    //コリジョンの最大サイズ
    const float COL_CHANGE_SPEED_ = 0.5f;     //コリジョンの大きさが変わるスピード
    const float COL_RESIZE_TIME_  = 2.2f;     //コリジョンの大きさが変わる時間
    const float COL_REMOVE_TIME_  = 4.0f;     //コリジョンを消す時間

    const float EFFECT_PLAY_SPEED_ = 3.0f;    //エフェクトの再生スピード
};

CEREAL_CLASS_VERSION(ComponentFireBall, 3);
