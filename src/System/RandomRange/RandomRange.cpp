//---------------------------------------------------------------------------
//!	@file	RandomRange.cpp
//! @brief	数字を指定した範囲でランダムに取得する
//! @author 田中南々子
//---------------------------------------------------------------------------
#include "Precompile.h"
#include "RandomRange.h"

//! @brief	指定した範囲のランダムな浮動小数点数を取得する
float GetRandomRangeF(float min, float max)
{
    float value  = max - min;
    float random = ((float)rand() / RAND_MAX) * value;

    return min + random;
}

//! @brief	指定した範囲のランダムな整数を取得する
int GetRandomRangeI(int min, int max)
{
    int value  = max - min;
    int random = rand() % (value + 1);

    return min + random;
}
