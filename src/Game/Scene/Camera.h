//---------------------------------------------------------------------------
//!	@file	Camera.h
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>

USING_PTR(Camera);

class Camera : public Object
{
public:
    BP_OBJECT_DECL(Camera, u8"プレイシーンのカメラ")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

private:
};
