//---------------------------------------------------------------------------
//!	@file	SoundBuffer.cpp
//! @brief	音のバッファクラス
//---------------------------------------------------------------------------
#include "SoundBuffer.h"
#include <DxLib.h>

std::unordered_map<std::string, int> SoundBuffer::sound_buffer_ = {};    // 音ハンドルのバッファを初期化

//---------------------------------------------------------------------------------

//!	初期化
//---------------------------------------------------------------------------------
void SoundBuffer::Init()
{
    sound_buffer_.clear();    // 音ハンドルのバッファをクリア

    sound_buffer_["mouse_click"] = DxLib::LoadSoundMem("data/PoyPoy/Sound/mouse_click.mp3");    // マウスクリック音声
    sound_buffer_["combo"]       = DxLib::LoadSoundMem("data/PoyPoy/Sound/combo.mp3");          // コンボ音声
    sound_buffer_["explosion"]   = DxLib::LoadSoundMem("data/PoyPoy/Sound/explosion.mp3");      // 爆発音声
    sound_buffer_["fire_ball"]   = DxLib::LoadSoundMem("data/PoyPoy/Sound/fireball.mp3");       // ファイアボール音声
    sound_buffer_["grave_hit"]   = DxLib::LoadSoundMem("data/PoyPoy/Sound/grave_hit.mp3");      // 墓石ヒット音声
    sound_buffer_["poison"]      = DxLib::LoadSoundMem("data/PoyPoy/Sound/poison.mp3");         // 毒音声
    sound_buffer_["rush"]        = DxLib::LoadSoundMem("data/PoyPoy/Sound/rush.mp3");           // ラッシュ音声

    sound_buffer_["title"] = DxLib::LoadSoundMem("data/PoyPoy/Sound/title.mp3");
    　　                                                                                          // タイトル音声
        sound_buffer_["game_scene"] = DxLib::LoadSoundMem("data/PoyPoy/Sound/game_scene.mp3");    // ゲームシーン音声
    sound_buffer_["result"]         = DxLib::LoadSoundMem("data/PoyPoy/Sound/result.mp3");        // リザルト音声
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
    if(sound_key.empty()) {
        return -1;    // キーが空の場合は無効なハンドルを返す
    }

    return sound_buffer_[sound_key];    // 音ハンドルを返す
}
