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
    bool Init() override;    //!< 初期化
};
