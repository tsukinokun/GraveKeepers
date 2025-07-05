//---------------------------------------------------------------------------
//!	@file	Wall.h
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>

USING_PTR(Wall);

class Wall : public Object
{
public:
    BP_OBJECT_DECL(Wall, u8"プレイシーンの壁")

    //@
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

private:
    const float RADUIS_ = 2.0f;
};
