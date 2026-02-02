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
#include <Game/Scene/UIObject/UIText.h>
#include <Game/System/HlslppUseful.h>

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
    // カメラを作成
    //---------------------------------------------------------------------------------
    {
        auto camera   = Scene::Object::Create<Object>();    //カメラ
        auto cam_comp = camera->AddComponent<ComponentCamera>();
        //座標と注視点の設定
        cam_comp->SetPositionAndTarget({0, 20, -30}, {0, 10, 0});
    }
    std::vector<std::weak_ptr<Object>> characters;    // キャラクターオブジェクト格納用

    //リザルトデータをソート
    std::sort(result_datas.begin(), result_datas.end(), [](const ResultInfo& a, const ResultInfo& b) { return a.rank_ < b.rank_; });

    //---------------------------------------------------------------------------------
    // キャラクターを表示(前から順に呼び出す)
    //---------------------------------------------------------------------------------
    for(const auto& result_data : result_datas) {
        auto        chara = Scene::Object::Create<Object>();
        std::string model_path;        // モデルパス格納用
        std::string walk_anim_path;    // 歩行アニメーションパス格納用
        u32         anim_index = 0;    // アニメーション番号
        if(result_data.chara_name_ == "Zombie") {
            model_path     = "data/PoyPoy/Model/Character/Zombie/Zombie.mv1";
            walk_anim_path = "data/PoyPoy/Model/Character/Zombie/Anims/Walking.mv1";
        }
        else if(result_data.chara_name_ == "Werewolf") {
            model_path     = "data/PoyPoy/Model/Character/Werewolf/Werewolf.mv1";
            walk_anim_path = "data/PoyPoy/Model/Character/Werewolf/Anims/Walk.mv1";
        }
        else if(result_data.chara_name_ == "Pumpking") {
            model_path     = "data/PoyPoy/Model/Character/Pumpking/Pumpking.mv1";
            walk_anim_path = "data/PoyPoy/Model/Character/Pumpking/Anims/Walk.mv1";
            anim_index     = 1;
        }
        else if(result_data.chara_name_ == "Witch") {
            model_path     = "data/PoyPoy/Model/Character/Witch/Witch.mv1";
            walk_anim_path = "data/PoyPoy/Model/Character/Witch/Anims/Walking.mv1";
        }
        //---------------------------------------------------------------------------------
        // キャラクターのモデルを追加
        //---------------------------------------------------------------------------------
        auto model_comp = chara->AddComponent<ComponentModel>(model_path);
        //---------------------------------------------------------------------------------
        //アニメーションも追加
        //---------------------------------------------------------------------------------
        model_comp->SetAnimation({
            {"walk", walk_anim_path, anim_index, 1.0f},
        });
        model_comp->PlayAnimation("walk", true);
        characters.push_back(chara);
    }
    //---------------------------------------------------------------------------------
    // いい感じの位置に配置する処理を書く
    //---------------------------------------------------------------------------------
    {
        for(int i = 0; i < characters.size(); i++) {
            if(auto character = characters.at(i).lock()) {
                float first_pos_x = -15.0f;    // 1stのX座標
                float x_offset    = 10.0f;     // キャラクター同士のX座標の間隔
                float x           = first_pos_x + (x_offset * i);
                character->SetTranslate(float3(x, 0.0f, 0.0f));
            }
        }
    }
    //---------------------------------------------------------------------------------
    // 1~4位であることを示す文字UIを生成
    //---------------------------------------------------------------------------------
    {
        const char* rank_texts[] = {"1st", "2nd", "3rd", "4th"};
        for(int i = 0; i < characters.size(); i++) {
            auto rank_ui = Scene::Object::Create<UIText>();
            rank_ui->SetText(rank_texts[i]);
            rank_ui->SetFontSize(48);
            rank_ui->SetColor(GetColor(255, 255, 255));
            // キャラクターの少し上に表示
            if(auto character = characters.at(i).lock()) {
                float           first_pos_x = (WINDOW_W / 2) - 550.0f;    // 1stのX座標
                constexpr float x_offset    = 275.0f;                     // 間隔
                float           x_pos       = first_pos_x + (x_offset * i);
                constexpr float y_pos       = 40.0f;    // Y座標
                rank_ui->SetTranslate(float3(x_pos, y_pos, 0.0f));
            }
        }
    }
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
