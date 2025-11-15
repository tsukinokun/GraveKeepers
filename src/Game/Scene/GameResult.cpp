//---------------------------------------------------------------------------
//! @file	GameResult.cpp
//! @brief	リザルト画面
//---------------------------------------------------------------------------
#include "GameResult.h"
#include "GameTitle.h"

//---------------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------------
bool GameResult::Init()
{
    __super::Init();

    // リザルト背景画像の読み込み
    result_back_graph = LoadGraph("data/PoyPoy/Image/Result.png");
    // 蜘蛛の巣画像の読み込み
    spider_web_graph = LoadGraph("data/PoyPoy/Image/SpiderWeb.png");

    return true;
}

//---------------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------------
void GameResult::Update()
{
    __super::Update();

    //SPACEキーが押されたらタイトル画面に移行
    if(CheckHitKey(KEY_INPUT_SPACE)) {
        Scene::Change(Scene::GetScene<GameTitle>());    //シーンの変更を行う処理
    }
}

//---------------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------------
void GameResult::Draw()
{
    __super::Draw();

    // リザルト背景画像を画面全体に描画
    if(result_back_graph != -1) {
        DrawExtendGraph(0, 0, WINDOW_W, WINDOW_H, result_back_graph, TRUE);
    }

    // 蜘蛛の巣画像を画面の四隅に描画
    if(spider_web_graph != -1) {
        DrawExtendGraph(0, 0, 200, 200, spider_web_graph, TRUE);                      // 左上
        DrawExtendGraph(WINDOW_W - 200, 0, WINDOW_W, 200, spider_web_graph, TRUE);    // 右上
    }

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

    // リザルト背景画像の解放
    if(result_back_graph != -1) {
        DeleteGraph(result_back_graph);
        result_back_graph = -1;
    }

    // 蜘蛛の巣画像の解放
    if(spider_web_graph != -1) {
        DeleteGraph(spider_web_graph);
        spider_web_graph = -1;
    }
}

//!GUI表示
void GameResult::GUI()
{
    __super::GUI();
    // ここにGUIの表示処理を追加
}
