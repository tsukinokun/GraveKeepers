//---------------------------------------------------------------------------
//!	@file	Witch.h
//! @brief	インゲームシーンの魔女
//! @author 上田朋輝
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <Game/Scene/Character/Base/Character.h>
USING_PTR(Witch);

class Witch : public Character
{
public:
    BP_OBJECT_DECL(Witch, u8"インゲームのゾンビ")

    //@
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

private:
    const int   HP_MAX_       = 180;     //HPの最大値
    const int   ATTACK_POWER_ = 30;      //攻撃力
    const float MOVE_SPEED_   = 0.3f;    //移動スピード
};
