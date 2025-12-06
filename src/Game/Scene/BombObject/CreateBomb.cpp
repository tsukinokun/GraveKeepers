//---------------------------------------------------------------------------
//!	@file	CreateBomb.cpp
//! @brief	爆弾を生成する処理
//! @author 田中南々子
//---------------------------------------------------------------------------
#include "CreateBomb.h"
#include "../../Scene/CandyBomb.h"

bool CreateBomb::Init()
{
    Super::Init();

    for(int i = 0; i < CANDYBOMB_NUM_MAX_; i++) {
        auto candybomb = Scene::Object::Create<CandyBomb>();
    }

    return true;
}

void CreateBomb::Update()
{
    Super::Update();

    //キャンディー爆弾の名前が付いたオブジェクトの数を取得
    int candybomb_num = Scene::Object::GetArray<CandyBomb>().size();
    //キャンディー爆弾の数が最大数未満なら最大数まで生成
    if(candybomb_num < CANDYBOMB_NUM_MAX_) {
        auto candybomb = Scene::Object::Create<CandyBomb>();
    }
}
