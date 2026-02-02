//---------------------------------------------------------------------------
//!	@file	Pumpking.h
//! @brief	インゲームシーンの狼男
//! @author
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <Game/Scene/Character/Base/Character.h>
USING_PTR(Pumpking);

class Pumpking : public Character
{
public:
    BP_OBJECT_DECL(Pumpking, u8"インゲームの狼男")

    //@
    bool Init() override;    //!< 初期化
    void GUI() override;     //!< GUI表示
    void Exit() override;    //!< 終了

private:
    const int   HP_MAX_       = 400;     //HPの最大値
    const int   ATTACK_POWER_ = 10;      //攻撃力
    const float MOVE_SPEED_   = 0.4f;    //移動スピード
};
