//---------------------------------------------------------------------------
//!	@file	SceneCharaSelect.cpp
//! @brief	キャラセレクトのシーン
//---------------------------------------------------------------------------
#include "SceneCharaSelect.h"
#include "Camera.h"
#include <Game/Scene/Character/Zombie/Zombie.h>
#include <Game/Scene/Character/Pumpking/Pumpking.h>
#include <Game/Scene/Character/Werewolf/Werewolf.h>
#include <Game/Scene/Character/Witch/Witch.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool SceneCharaSelect::Init()
{
    __super::Init();
    auto camera = Scene::Object::Create<Camera>();    //カメラ
    //とりあえず、全員表示させておく(仮で)
    Scene::Object::Create<Zombie>();
    Scene::Object::Create<Pumpking>();
    Scene::Object::Create<Werewolf>();
    Scene::Object::Create<Witch>();
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void SceneCharaSelect::Update()
{
    __super::Update();
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void SceneCharaSelect::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void SceneCharaSelect::Exit()
{
    __super::Exit();
    // ここにゲームの終了処理を追加
}

//!GUI表示
void SceneCharaSelect::GUI()
{
    __super::GUI();
    // ここにGUIの表示処理を追加
}
