//---------------------------------------------------------------------------
//!	@file	Zombie.h
//! @brief	インゲームシーンのゾンビ
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <Game/Scene/Character/Base/Character.h>
USING_PTR(Zombie);

class Zombie : public Character
{
public:
    BP_OBJECT_DECL(Zombie, u8"インゲームのゾンビ")

    //@
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

private:
    const int HP_MAX_ = 200;    //HPの最大値
};
