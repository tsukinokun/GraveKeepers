//---------------------------------------------------------------------------
//!	@file	CreateBlock.h
//! @brief	ブロックの生成
//! @author 田中南々子
//---------------------------------------------------------------------------

#pragma once
#include <System/Scene.h>
//前方宣言
USING_PTR(CreateBlock);

class CreateBlock : public Object
{
public:
    BP_OBJECT_DECL(CreateBlock, u8"ブロックオブジェクトの生成")
    //@
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新

private:
    const int BLOCK_NUM_MAX_ = 10;
};
