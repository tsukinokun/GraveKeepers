//---------------------------------------------------------------------------
//!	@file	SoundBuffer.cpp
//! @brief	音のバッファクラス
//---------------------------------------------------------------------------
#include "SoundBuffer.h"

std::unordered_map<std::string, int> SoundBuffer::sound_buffer_ = {};    // 音ハンドルのバッファを初期化
std::unordered_map<std::string, int> SoundBuffer::bgm_buffer_   = {};    // 音ハンドルのバッファを初期化

//---------------------------------------------------------------------------------

//!	初期化
//---------------------------------------------------------------------------------
void SoundBuffer::Init()
{
    sound_buffer_.clear();                                                          // 音ハンドルのバッファをクリア
    bgm_buffer_.clear();                                                            // 音ハンドルのバッファをクリア
    bgm_buffer_["result"] = DxLib::LoadSoundMem("data/PoyPoy/Sound/result.mp3");    // リザルト音声
}

//---------------------------------------------------------------------------------
//!	終了処理
//---------------------------------------------------------------------------------
void SoundBuffer::Exit()
{
    // 音ハンドルを解放
    for(auto it = sound_buffer_.begin(); it != sound_buffer_.end(); ++it) {
        if(it->second != -1) {
            DxLib::DeleteSoundMem(it->second);    // 音ハンドルを解放
            it->second = -1;                      // ハンドルを無効化
        }
    }
}

//---------------------------------------------------------------------------------
//!	音ハンドルを取得する関数
//---------------------------------------------------------------------------------
int SoundBuffer::GetSoundHandle(const std::string& sound_key)
{
    // 音ハンドルを取得
    //音が存在しない場合はエラーになる可能性ありから-1を返す
    if(sound_buffer_.find(sound_key) == sound_buffer_.end()) {
        return -1;
    }

    return sound_buffer_[sound_key];    // 音ハンドルを返す
}

int SoundBuffer::GetBGMHandle(const std::string& sound_key)
{
    //音が存在しない場合はエラーになる可能性ありから-1を返す
    if(bgm_buffer_.find(sound_key) == bgm_buffer_.end()) {
        return -1;
    }

    return bgm_buffer_[sound_key];    // 音ハンドルを返す
}
