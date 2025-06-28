//---------------------------------------------------------------------------
//!	@file	ScenePlay.cpp
//! @brief	ゲームメイン
//---------------------------------------------------------------------------
#include "ScenePlay.h"
#include "Player.h"
#include "Enemy.h"
#include "Camera.h"
#include "Field.h"
#include "Block.h"
#include "UFO.h"

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

    for(int i = 0; i < BLOCK_NUM_MAX_; i++) {
        auto block = Scene::Object::Create<Block>();
    }

    auto ufo = Scene::Object::Create<UFO>();

    auto camera = Scene::Object::Create<Camera>();

    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void ScenePlay::Update()
{
    __super::Update();
    // ここにゲームの更新処理を追加
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void ScenePlay::Draw()
{
    __super::Draw();

    // ここにゲームの描画処理を追加
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
