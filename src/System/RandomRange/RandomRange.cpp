//---------------------------------------------------------------------------
//!	@file	RandomRange.cpp
//! @brief	数字を指定した範囲でランダムに取得する
//! @author 田中南々子
//---------------------------------------------------------------------------
#include "RandomRange.h"

//! @brief	指定した範囲のランダムな浮動小数点数を取得する
float GetRandomRangeF(float min, float max)
{
    // min以上max未満の範囲でランダムな浮動小数点数を取得する
    // rand()関数は0からRAND_MAXまでの整数を返す
    // これを0.0から1.0の範囲に正規化し、指定した範囲にスケーリングする
    // そしてminを加算して最終的な範囲にシフトする
    float value  = max - min;
    float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * value;
    return min + random;
}

//! @brief	指定した範囲のランダムな整数を取得する
int GetRandomRangeI(int min, int max)
{
    // min以上max未満の範囲でランダムな整数を取得する
    // rand()関数は0からRAND_MAXまでの整数を返す
    // これを0から(max - min)の範囲にスケーリングし、minを加算して最終的な範囲にシフトする
    int value  = max - min;
    int random = rand() % value;    // 0から(value - 1)の範囲の整数を取得
    return min + random;
}
