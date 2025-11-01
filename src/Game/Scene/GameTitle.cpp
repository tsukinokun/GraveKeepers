//---------------------------------------------------------------------------
//! @file	GameTitle.cpp
//! @brief	タイトル画面
//---------------------------------------------------------------------------
#include "GameTitle.h"
#include "ScenePlay.h"

//---------------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------------
bool GameTitle::Init()
{
    __super::Init();

    //背景画像の読み込み
    title_back_graph = LoadGraph("data/PoyPoy/Image/Title.png");

    return true;
}

//---------------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------------
void GameTitle::Update()
{
    __super::Update();

    //SPACEキーが押されたらゲーム画面に移行
    if(CheckHitKey(KEY_INPUT_SPACE)) {
        Scene::Change(Scene::GetScene<ScenePlay>());    //シーンの変更を行う処理
    }
}

//---------------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------------
void GameTitle::Draw()
{
    __super::Draw();

    // 背景画像の描画（ウィンドウ全体にフィット）
    if(title_back_graph != -1) {
        DrawExtendGraph(0, 0, WINDOW_W, WINDOW_H, title_back_graph, TRUE);
    }

    // 文字のサイズを設定
    SetFontSize(64);

    // ゲーム画面に合わせて画面中央上部にタイトル名を表示（上部にぴったり）
    const char* title = "ポイポイ";
    DrawString((WINDOW_W - GetDrawStringWidth(title, (int)strlen(title))) / 2, 20, title, GetColor(255, 255, 255));

    // ゲーム画面に合わせて画面中央に「SPACEキーでスタート」と表示
    const char* msg = "SPACEキーでスタート";
    DrawString((WINDOW_W - GetDrawStringWidth(msg, (int)strlen(msg))) / 2, WINDOW_H / 2 + 30, msg, GetColor(255, 255, 255));
}

//---------------------------------------------------------------------------------
//! 終了
//---------------------------------------------------------------------------------
void GameTitle::Exit()
{
    __super::Exit();

    //　背景画像の削除
    if(title_back_graph != -1) {
        DeleteGraph(title_back_graph);
        title_back_graph = -1;
    }
}

//!GUI表示
void GameTitle::GUI()
{
    __super::GUI();
    // ここにGUIの表示処理を追加
}
