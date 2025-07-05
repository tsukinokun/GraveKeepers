//---------------------------------------------------------------------------
//!	@file	ScenePlay.cpp
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#include "Player.h"
#include "ScenePlay.h"
#include <chrono>
#include "Enemy.h"
#include "Camera.h"
#include "Field.h"
#include "Block.h"
#include "UFO.h"
#include "Wall.h"

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ScenePlay::Init()
{
    __super::Init();

    auto field = Scene::Object::Create<Field>();

    auto player = Scene::Object::Create<Player>();

    for(int i = 0; i < ENEMY_MAX_; i++) {
        auto enemy = Scene::Object::Create<Enemy>();
    }

    previousTime_ = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < BLOCK_NUM_MAX_; i++) {
        auto block = Scene::Object::Create<Block>();
    }

    auto ufo = Scene::Object::Create<UFO>();

    auto camera = Scene::Object::Create<Camera>();

    //四方向に壁を生成
    for(int i = 0; i < 4; i++) {
        auto wall = Scene::Object::Create<Wall>();
        wall->SetTranslate(WALL_POS_[i]);
        if(i == 2 || i == 3) {
            wall->AddRotationAxisXYZ(float3(0.0f, 90.0f, 0.0f));
        }
    }
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void ScenePlay::Update()
{
    __super::Update();
    // ここにゲームの更新処理を追加
    //プレイヤーのHPをカメラに与える
    auto player = Scene::Object::Get<Player>(u8"プレイヤー");
    auto camera = Scene::Object::Get<Camera>("Camera");

    camera->GetPlayerHP(player->GetHP());
    camera->GetPlayerHP(player->GetHP());
    int enemy_num = 0;
    for(auto enemy : Scene::Object::GetArray<Enemy>()) {
        //エネミーのHPをカメラに与える
        camera->GetEnemyHP(enemy->GetHP(), enemy_num);
        enemy_num++;
    }
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void ScenePlay::Draw()
{
    __super::Draw();

    // ここにゲームの描画処理を追加
    //文字の回りに黒い縁を追加
    ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);

    // 時間差分を計算
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto deltaTime   = currentTime - previousTime_;
    previousTime_    = currentTime;

    // タイマーを減算（カウントダウン）
    TIMER_COUNT_ -= std::chrono::duration<float>(deltaTime).count();
    if(TIMER_COUNT_ < 0.0f) {
        TIMER_COUNT_ = 0.0f;
    }

    // 分と秒に変換（ゼロ埋め付き表示）
    int minutes = static_cast<int>(TIMER_COUNT_) / 60;
    int seconds = static_cast<int>(TIMER_COUNT_) % 60;

    // タイマーを画面に描画（DxLib関数）
    DrawFormatString(100, 50, GetColor(255, 255, 0), "%02d:%02d", minutes, seconds);
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void ScenePlay::Exit()
{
    __super::Exit();
    // ここにゲームの終了処理を追加
}

//!GUI表示
void ScenePlay::GUI()
{
    __super::GUI();
    // ここにGUIの表示処理を追加
}
