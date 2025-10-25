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
#include "Block.h"
#include "UFO.h"
#include "Wall.h"
#include "CandyBomb.h"
#include "Sky.h"

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
    ImageBuffer::Init();    // 画像バッファの初期化
    //文字の回りに黒い縁を追加
    ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);

    auto field = Scene::Object::Create<Field>();

    auto player = Scene::Object::Create<Player>();

    for(int i = 0; i < ENEMY_MAX_; i++) {
        auto enemy = Scene::Object::Create<Enemy>();
    }

    previous_time_ = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < BLOCK_NUM_MAX_; i++) {
        auto block = Scene::Object::Create<Block>();
    }

    for(int i = 0; i < CANDYBOMB_NUM_MAX_; i++) {
        auto candybomb = Scene::Object::Create<CandyBomb>();
    }

    auto ufo = Scene::Object::Create<UFO>();

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

    //---------------------------------------------------------------------------------
    //	タイマー処理
    //---------------------------------------------------------------------------------
    // タイマーを減算（カウントダウン）
    TIMER_COUNT_ -= std::chrono::duration<float>(delta_time).count();
    if(TIMER_COUNT_ < 0.0f) {
        TIMER_COUNT_ = 0.0f;
    }
    // 分と秒に変換（ゼロ埋め付き表示）
    int minutes = static_cast<int>(TIMER_COUNT_) / 60;
    int seconds = static_cast<int>(TIMER_COUNT_) % 60;
    //タイマーUIの更新
    if(auto timer_ui = Scene::Object::Get<UIText>(u8"タイマーUI")) {
        timer_ui->SetText(std::to_string(minutes) + ":" + std::to_string(seconds));
    }

    // ここにゲームの更新処理を追加
    //プレイヤーのHPをカメラに与える
    auto player = Scene::Object::Get<Player>(u8"プレイヤー");
    auto camera = Scene::Object::Get<Camera>("Camera");

    //camera->GetPlayerHP(player->GetComponent<ComponentStatus>()->GetHitPoints());
    int enemy_num = 0;
    //for(auto enemy : Scene::Object::GetArray<Enemy>())
    //{
    //	//エネミーのHPをカメラに与える
    //	camera->GetEnemyHP(enemy->GetComponent<ComponentStatus>()->GetHitPoints(), enemy_num);
    //	enemy_num++;
    //}

    //---------------------------------------------------------------------------------
    //	HPのUIオブジェクト
    //---------------------------------------------------------------------------------
    for(int i = 0; i < CHARACTER_ALL; i++) {
        std::string name  = "HP";                 //HPテキストオブジェクトの名前
        name             += std::to_string(i);    //オブジェクト名をHP0、HP1、HP2、HP3とする
        if(auto hp_ui = Scene::Object::Get<UIText>(name)) {
            //キャラクター名からキャラを取得し、
            std::string chara_name = "Character";
            //二体目以降の命名規則
            if(i != 0) {
                chara_name += "_" + std::to_string(i);
            }
            if(auto chara = Scene::Object::Get<Object>(chara_name)) {
                hp_ui->SetText(std::to_string(chara->GetComponent<ComponentStatus>()->GetHitPoints()));    //キャラのHPをUIに反映
            }
        }
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
