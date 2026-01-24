//---------------------------------------------------------------------------
//! @file	GameResult.cpp
//! @brief	リザルト画面
//---------------------------------------------------------------------------
#include "GameResult.h"
#include "GameTitle.h"
#include <Game/System/GameRepository.h>
#include <Game/System/ImageBuffer.h>
#include <Game/System/SoundBuffer.h>
#include <Game/Scene/info/ResultInfo.h>
#include <System/Component/ComponentModel.h>

//---------------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------------
bool GameResult::Init()
{
    __super::Init();

    //---------------------------------------------------------------------------------
    // リポジトリ(シングルトン)の呼び出し
    //---------------------------------------------------------------------------------
    auto result_datas = GameRepository::Instance().GetResultDatas();
    ImageBuffer::Init();    // 画像バッファの初期化
    SoundBuffer::Init();    // 音バッファの初期化

    //---------------------------------------------------------------------------------
    // キャラクターを表示(前から順に呼び出す)
    //---------------------------------------------------------------------------------
    for(const auto& result_data : result_datas) {
        auto chara = Scene::Object::Create<Object>();
        if(result_data.chara_name_ == "Zombie") {
            // ゾンビキャラクターのモデルを追加
            chara->AddComponent<ComponentModel>("data/PoyPoy/Model/Character/Zombie/Zombie.mv1");
            //アニメーションも追加
        }
        else if(result_data.chara_name_ == "Werewolf") {
            chara->AddComponent<ComponentModel>("data/PoyPoy/Model/Character/Werewolf/Werewolf.mv1");
        }
    }
    //---------------------------------------------------------------------------------
    // いい感じの位置に配置する処理を書く
    //---------------------------------------------------------------------------------

    return true;
}

//---------------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------------
void GameResult::Update()
{
    __super::Update();

    //  リザルト音声の再生
    static bool is_played = false;
    if(!is_played) {
        int result_sound = SoundBuffer::GetBGMHandle("result");
        PlaySoundMem(result_sound, DX_PLAYTYPE_LOOP);
        is_played = true;
    }

    //SPACEキーが押されたらタイトル画面に移行
    if(IsKeyOn(KEY_INPUT_SPACE)) {
        Scene::Change(Scene::GetScene<GameTitle>());    //シーンの変更を行う処理

        //音楽が流れていたら停止
        int result_sound = SoundBuffer::GetBGMHandle("result");
        StopSoundMem(result_sound);
        is_played = false;
    }
}

//---------------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------------
void GameResult::Draw()
{
    __super::Draw();

    // 画面全体にリザルト背景画像を描画
    int result_back_graph = ImageBuffer::GetImageHandle("result");
    DrawExtendGraph(0, 0, WINDOW_W, WINDOW_H, result_back_graph, TRUE);

    // 文字のサイズを設定
    SetFontSize(64);
    // 画面中央上部に「RESULT」と表示
    const char* result = "RESULT";
    DrawString((WINDOW_W / 2) - (GetDrawStringWidth(result, (int)strlen(result)) / 2), 20, result, GetColor(255, 255, 255));

    // 1st,2nd,3rd,4thを等間隔に横一列に表示
    const char* rank_texts[] = {"1st", "2nd", "3rd", "4th"};
    int         rank_count   = sizeof(rank_texts) / sizeof(rank_texts[0]);
    for(int i = 0; i < rank_count; ++i) {
        // 文字のサイズを設定(1stは大きく、他は普通)
        if(i == 0) {
            SetFontSize(48);
        }
        else {
            SetFontSize(32);
        }
        // 各ランクの表示位置を計算して描画
        int x_pos = (WINDOW_W / (rank_count + 1)) * (i + 1) - (GetDrawStringWidth(rank_texts[i], (int)strlen(rank_texts[i])) / 2);
        int y_pos = WINDOW_H / 2;
        DrawString(x_pos, y_pos, rank_texts[i], GetColor(255, 255, 255));
    }
}

//---------------------------------------------------------------------------------
//! 終了
//---------------------------------------------------------------------------------
void GameResult::Exit()
{
    __super::Exit();
}

//!GUI表示
void GameResult::GUI()
{
    __super::GUI();
    // ここにGUIの表示処理を追加
}
