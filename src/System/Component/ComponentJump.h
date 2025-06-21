#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentJump);

class ComponentJump : public Component
{
public:
    BP_COMPONENT_DECL(ComponentJump, u8"ジャンプコンポーネント");

    void Init() override;

    void Update() override;

    void GUI() override;

    void SetJumpFrame(int value);    //何フレームジャンプを行うかをセット

    void SetJumpHight(float value);    //ジャンプでどのくらい飛び上がるか

    void NotJump();    //ジャンプをできないようにするための関数

    bool IsJump();    //ジャンプをしているかを返す関数

private:
    int   jump_frame_max_   = 60;    //何フレームジャンプするか
    int   jump_frame_count_ = 0;     //ジャンプ中のフレーム
    float jump_hight_       = 5;     //ジャンプで飛び上がる高さ
    float translate_hight_  = 0;     //実際にmatrixに入れる値

    bool not_jump_ = false;    //ジャンプをできるかどうかの変数

    bool is_jump_ = false;    //ジャンプをしているかの変数

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
