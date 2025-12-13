//---------------------------------------------------------------------------
//!	@file	CreateBlock.cpp
//! @brief	ブロックの生成
//! @author 田中南々子
//---------------------------------------------------------------------------
#include "CreateBlock.h"
#include "../../Scene/Block.h"

bool CreateBlock::Init()
{
    Super::Init();

    for(int i = 0; i < BLOCK_NUM_MAX_; i++) {
        auto candybomb = Scene::Object::Create<Block>();
    }

    return true;
}

void CreateBlock::Update()
{
    Super::Update();
    //ブロックの名前が付いたオブジェクトの数を取得
    int block_num = Scene::Object::GetArray<Block>().size();
    //ブロックの数が最大数未満なら最大数まで生成
    if(block_num < BLOCK_NUM_MAX_) {
        auto block = Scene::Object::Create<Block>();
    }
}
