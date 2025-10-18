//---------------------------------------------------------------------------
//!	@file	FontBuffer.h
//! @brief	フォントのバッファクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include "FontKey.h"

class FontBuffer
{
public:
    static void Init();    //初期化処理
    static void Exit();    //終了処理

    //フォントハンドルを取得する関数
    //! @param [in] 取得するフォントのキー
    //! @retval フォントハンドル
    int GetFontHandle(const std::string& fontName, int size, int thickness = 1, int type = DX_FONTTYPE_NORMAL);

private:
    static std::unordered_map<FontKey, int> font_buffer_;    //フォントハンドルのバッファ
};
