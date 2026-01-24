//---------------------------------------------------------------------------
//!	@file	SoundBuffer.h
//! @brief	音のバッファクラス
//! @author 上田朋輝
//---------------------------------------------------------------------------
#pragma once

class SoundBuffer
{
public:
    static void Init();    //初期化処理
    static void Exit();    //終了処理

    //画像ハンドルを取得する関数
    //! @param [in] 取得する音のキー
    //! @retval 音ハンドル
    static int GetSoundHandle(const std::string& sound_key);
    static int GetBGMHandle(const std::string& sound_key);

private:
    static std::unordered_map<std::string, int> sound_buffer_;    //音ハンドルのバッファ
    static std::unordered_map<std::string, int> bgm_buffer_;      //音ハンドルのバッファ
};
