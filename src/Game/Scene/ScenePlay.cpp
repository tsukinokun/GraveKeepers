//---------------------------------------------------------------------------
//!	@file	ScenePlay.cpp
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#include "Player.h"
#include "ScenePlay.h"
#include <chrono>
#include <Game/Scene/Character/Base/Character.h>
#include <Game/Scene/UIObject/UIText.h>
#include <System/UIComponent/ComponentText.h>
#include <Game/Scene/UIObject/UIImage.h>
#include <System/Component/ComponentStatus.h>
#include <Game/System/ImageBuffer.h>
#include <Game/System/common.h>
#include "Enemy.h"
#include "Camera.h"
#include "Field.h"
#include "Wall.h"
#include "Sky.h"
#include "GameResult.h"
#include "BombObject/CreateBomb.h"
#include "BlockObject/CreateBlock.h"
#include <Game/System/GameRepository.h>
#include <Game/Scene/Character/CharacterFactory.h>
#include <algorithm>
#include <random>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ScenePlay::Init()
{
    // フォントを使用できるように登録（アプリ内限定）
    int result = AddFontResourceEx("data/PoyPoy/Font/Hyakki85_OTF/Hyakki85.otf", FR_PRIVATE, NULL);

    if(result == 0) {
        MessageBox(NULL, "フォント読み込み失敗", "エラー", MB_OK);
    }
    __super::Init();
    //文字の回りに黒い縁を追加
    ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);

    auto field = Scene::Object::Create<Field>();

    auto player = Scene::Object::Create<Player>();
    characters_.push_back(player->GetControllCharacter());    //キャラクターオブジェクトの配列に追加

    // プレイヤーが選んだキャラを除いた候補を取得
    auto names           = CharacterFactory::Instance().GetRegisteredCharacterNames();
    auto player_selected = GameRepository::Instance().GetSelectedCharacterName();
    names.erase(std::remove(names.begin(), names.end(), player_selected), names.end());

    // シャッフルして上から3つを使う（候補が3未満ならある分だけ）
    std::random_device rd;
    std::mt19937       g(rd());

    int                      spawnCount = std::min<int>(3, static_cast<int>(names.size()));
    std::vector<std::string> selected;

    // 3人が同じ名前なら再抽選
    while(true) {
        std::shuffle(names.begin(), names.end(), g);

        selected.clear();
        for(int i = 0; i < spawnCount; i++) {
            selected.push_back(names[i]);
        }

        // 全部同じなら再抽選
        bool allSame = true;
        for(int i = 1; i < spawnCount; i++) {
            if(selected[i] != selected[0]) {
                allSame = false;
                break;
            }
        }

        if(!allSame)
            break;
    }

    // NPC生成
    for(int i = 0; i < spawnCount; i++) {
        auto enemy = Scene::Object::Create<Enemy>();
        enemy->SetDesiredCharacterName(selected[i]);
        characters_.push_back(enemy->GetControllCharacter());
    }

    previous_time_ = std::chrono::high_resolution_clock::now();

    auto block = Scene::Object::Create<CreateBlock>();

    auto candybomb = Scene::Object::Create<CreateBomb>();

    auto camera = Scene::Object::Create<Camera>();

    //四方向に壁を生成
    for(int i = 0; i < 4; i++) {
        auto wall = Scene::Object::Create<Wall>();
        wall->SetTranslate(WALL_POS_[i]);
        if(i == 2 || i == 3) {
            wall->SetRotationAxisXYZ(float3(90.0f, 0.0f, 0.0f));
        }
    }

    auto sky = Scene::Object::Create<Sky>();

    //---------------------------------------------------------------------------------
    //	時間UIオブジェクトの設定
    //---------------------------------------------------------------------------------
    auto timer_ui = Scene::Object::Create<UIText>(u8"タイマーUI");
    timer_ui->SetTranslate(float3(100.0f, 50.0f, 0.0f));
    timer_ui->SetText("00:00");
    timer_ui->SetColor(GetColor(255, 255, 0));    // 色を黄色に設定
    //---------------------------------------------------------------------------------
    //	HPのUIオブジェクト
    //---------------------------------------------------------------------------------
    for(int i = 0; i < CHARACTER_ALL; i++) {
        std::string name  = "HP";
        name             += std::to_string(i);    //オブジェクト名をHP0、HP1、HP2、HP3とする
        auto hp_ui        = Scene::Object::Create<UIText>(name);
        hp_ui->SetFontSize(HP_FONT_SIZE);          //フォントサイズ
        hp_ui->SetText("888");                     //HPテキスト、三桁が最大
        hp_ui->SetColor(GetColor(0, 255, 255));    //文字色は水色に
        hp_ui->SetTranslate(float3(HP_POS_X[i], HP_POS_Y, 0.0f));
    }
    //---------------------------------------------------------------------------------
    //	テスト画像
    //---------------------------------------------------------------------------------
    auto test_image = Scene::Object::Create<UIImage>(u8"テスト画像");
    test_image->SetTranslate(float3(WINDOW_W / 2, WINDOW_H / 2, 0.0f));
    test_image->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
    test_image->SetScaleAxisXYZ(0.1f);
    test_image->SetImage(ImageBuffer::GetImageHandle("test"));    // 画像ハンドルを設定
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void ScenePlay::Update()
{
    __super::Update();
    // 時間差分を計算
    auto current_time = std::chrono::high_resolution_clock::now();
    auto delta_time   = current_time - previous_time_;
    previous_time_    = current_time;

    int allive_count = 0;
    //生存しているキャラクターの数をカウント
    for(auto& weak_chara : characters_) {
        if(auto chara = weak_chara.lock()) {
            if(chara->GetComponent<ComponentStatus>()->IsDead() == false) {
                allive_count++;
            }
        }
    }
    //---------------------------------------------------------------------------------
    //	HPのUIオブジェクト
    //---------------------------------------------------------------------------------
    for(int i = 0; i < CHARACTER_ALL; i++) {
        if(auto chara = characters_[i].lock()) {
            if(auto status_comp = chara->GetComponent<ComponentStatus>()) {
                if(result_datas_[i].rank_ == -1) {
                    //順位が未設定の場合、生存しているキャラクターの数+1を順位として設定
                    if(status_comp->IsDead()) {
                        result_datas_[i].rank_       = allive_count + 1;           //順位を設定
                        result_datas_[i].chara_name_ = chara->GetNameDefault();    //キャラの名前を設定
                    }
                }

                std::string name  = "HP";                 //HPテキストオブジェクトの名前
                name             += std::to_string(i);    //オブジェクト名をHP0、HP1、HP2、HP3とする
                if(auto hp_ui = Scene::Object::Get<UIText>(name)) {
                    hp_ui->SetText(std::to_string(status_comp->GetHitPoints()));    //キャラのHPをUIに反映
                }
            }
        }
    }
    //---------------------------------------------------------------------------------
    //	タイマー処理
    //---------------------------------------------------------------------------------
    // タイマーを減算（カウントダウン）
    TIMER_COUNT_ -= std::chrono::duration<float>(delta_time).count();

    //---------------------------------------------------------------------------------
    // 	ゲーム終了判定
    //---------------------------------------------------------------------------------
    // 生存しているキャラクターが1人以下、またはタイマーが0以下になったらゲーム終了
    if(allive_count <= 1) {
        for(int i = 0; i < result_datas_.size(); i++) {
            //順位が未設定のキャラクターに1位を設定
            if(result_datas_[i].rank_ == -1) {
                result_datas_[i].rank_ = 1;    //順位を設定
            }
        }
        GameRepository::Instance().SetResultDatas(result_datas_);    //結果データをGameRepositoryに設定
        Scene::Change(Scene::GetScene<GameResult>());                //シーンの変更を行う処理
    }
    else if(TIMER_COUNT_ < 0.0f) {
        // 生存しているキャラクターを、HPの多い順に順位付け

        std::vector<std::pair<int, int>> hp_ranks;    //キャラクターのインデックスとHPのペア配列
        for(int i = 0; i < characters_.size(); i++) {
            if(auto chara = characters_[i].lock()) {
                if(auto status_comp = chara->GetComponent<ComponentStatus>()) {
                    hp_ranks.push_back(std::make_pair(i, status_comp->GetHitPoints()));
                }
            }
        }
        // HPの多い順にソート
        std::sort(hp_ranks.begin(), hp_ranks.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) { return a.second < b.second; });
        int current_rank = 4;    //4位から順位を設定
        for(auto hp_rank : hp_ranks) {
            int idx = hp_rank.first;
            //順位が未設定のキャラクターに順位を設定
            if(result_datas_[idx].rank_ == -1) {
                result_datas_[idx].rank_ = current_rank;    //順位を設定
                if(auto chara = characters_[idx].lock()) {
                    result_datas_[idx].chara_name_ = chara->GetNameDefault();    //キャラの名前を設定
                }
                current_rank--;
            }
        }
        GameRepository::Instance().SetResultDatas(result_datas_);    //結果データをGameRepositoryに設定
        Scene::Change(Scene::GetScene<GameResult>());                //シーンの変更を行う処理
    }
    // 分と秒に変換（ゼロ埋め付き表示）
    int minutes = static_cast<int>(TIMER_COUNT_) / 60;
    int seconds = static_cast<int>(TIMER_COUNT_) % 60;
    //タイマーUIの更新
    if(auto timer_ui = Scene::Object::Get<UIText>(u8"タイマーUI")) {
        timer_ui->SetText(std::to_string(minutes) + ":" + std::to_string(seconds));
    }
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void ScenePlay::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void ScenePlay::Exit()
{
    __super::Exit();
    // ここにゲームの終了処理を追加
    ImageBuffer::Exit();    // 画像バッファの終了処理
}

//!GUI表示
void ScenePlay::GUI()
{
    __super::GUI();
    // ここにGUIの表示処理を追加
}
