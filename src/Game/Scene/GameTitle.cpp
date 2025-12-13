//---------------------------------------------------------------------------
//! @file	GameTitle.cpp
//! @brief	タイトル画面
//---------------------------------------------------------------------------
#include "GameTitle.h"
#include "ScenePlay.h"
#include "SceneCharaSelect.h"
#include <Game/Scene/Character/CharacterFactory.h>
#include <Game/System/GameRepository.h>
#include "UIObject/UIText.h"
#include <System/Component/ComponentModel.h>

//---------------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------------
bool GameTitle::Init()
{
    __super::Init();

    //背景画像の読み込み
    title_back_graph = LoadGraph("data/PoyPoy/Image/Title.png");
    //---------------------------------------------------------------------------------
    // シングルトンのインスタンスを生成しておく(インゲームで読み込まなくてよいようにここで読み込んでおく)
    //---------------------------------------------------------------------------------
    GameRepository::Instance();
    CharacterFactory::Instance();
    //---------------------------------------------------------------------------------
    // カメラを作成
    //---------------------------------------------------------------------------------
    {
        auto camera   = Scene::Object::Create<Object>();    //カメラ
        auto cam_comp = camera->AddComponent<ComponentCamera>();
        //座標と注視点の設定
        cam_comp->SetPositionAndTarget({0, 20, 30}, {0, 10, 0});
    }
    std::vector<std::weak_ptr<Object>> characters;    // キャラクターオブジェクト格納用
    //---------------------------------------------------------------------------------
    //	タイトルのUIオブジェクト
    //---------------------------------------------------------------------------------
    auto title_ui = Scene::Object::Create<UIText>(u8"タイトルUI");
    title_ui->SetFontSize(64);
    title_ui->SetText("グレイブキーパーズ");
    title_ui->SetColor(GetColor(255, 255, 0));
    title_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
    title_ui->SetTranslate(float3(WINDOW_W * 0.05f, WINDOW_H * 0.05f, 0.0f));

    //---------------------------------------------------------------------------------
    // 各キャラクターを作成
    //---------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------
    // ゾンビ
    //---------------------------------------------------------------------------------
    {
        auto zombie = Scene::Object::Create<Object>();
        zombie->SetTranslate(float3(0.0f, 0.0f, 0.0f));
        zombie->SetRotationAxisXYZ(float3(0.0f, 180.0f, 0.0f));
        zombie->SetName(u8"Zombie");
        //---------------------------------------------------------------------------------
        //モデルコンポーネントの設定
        //---------------------------------------------------------------------------------
        auto model_comp = zombie->AddComponent<ComponentModel>();
        model_comp->Load("data/PoyPoy/Model/Character/Zombie/Zombie.mv1");
        model_comp->SetAnimation({
            {"walk", "data/PoyPoy/Model/Character/Zombie/Anims/Walking.mv1", 0, 1.0f},
        });
        model_comp->PlayAnimation("walk", true);
        characters.push_back(zombie);
    }
    //---------------------------------------------------------------------------------
    // 狼男
    //---------------------------------------------------------------------------------
    {
        auto wolf = Scene::Object::Create<Object>();
        wolf->SetTranslate(float3(0.0f, 0.0f, 0.0f));
        wolf->SetRotationAxisXYZ(float3(0.0f, 180.0f, 0.0f));
        wolf->SetName(u8"Werewolf");
        //---------------------------------------------------------------------------------
        //モデルコンポーネントの設定
        //---------------------------------------------------------------------------------
        auto model_comp = wolf->AddComponent<ComponentModel>();
        model_comp->Load("data/PoyPoy/Model/Character/Werewolf/Werewolf.mv1");
        model_comp->SetAnimation({
            {"walk", "data/PoyPoy/Model/Character/Werewolf/Anims/Walk.mv1", 0, 1.0f},
        });
        model_comp->PlayAnimation("walk", true);
        characters.push_back(wolf);
    }
    //---------------------------------------------------------------------------------
    // パンプキング
    //---------------------------------------------------------------------------------
    {
        auto pumpkin = Scene::Object::Create<Object>();
        pumpkin->SetTranslate(float3(0.0f, 0.0f, 0.0f));
        pumpkin->SetRotationAxisXYZ(float3(0.0f, 180.0f, 0.0f));
        pumpkin->SetName(u8"Pumpking");
        //---------------------------------------------------------------------------------
        //モデルコンポーネントの設定
        //---------------------------------------------------------------------------------
        auto model_comp = pumpkin->AddComponent<ComponentModel>();
        model_comp->Load("data/PoyPoy/Model/Character/Pumpking/Pumpking.mv1");
        model_comp->SetAnimation({
            {"walk", "data/PoyPoy/Model/Character/Pumpking/Anims/Walk.mv1", 1, 1.0f},
        });
        model_comp->PlayAnimation("walk", true);
        characters.push_back(pumpkin);
    }
    //---------------------------------------------------------------------------------
    // ウィッチ
    //---------------------------------------------------------------------------------
    {
        auto witch = Scene::Object::Create<Object>();
        witch->SetTranslate(float3(0.0f, 0.0f, 0.0f));
        witch->SetRotationAxisXYZ(float3(0.0f, 180.0f, 0.0f));
        witch->SetName(u8"Witch");
        //---------------------------------------------------------------------------------
        //モデルコンポーネントの設定
        //---------------------------------------------------------------------------------
        auto model_comp = witch->AddComponent<ComponentModel>();
        model_comp->Load("data/PoyPoy/Model/Character/Witch/Witch.mv1");
        model_comp->SetAnimation({
            {"walk", "data/PoyPoy/Model/Character/Witch/Anims/Walking.mv1", 0, 1.0f},
        });
        model_comp->PlayAnimation("walk", true);
        characters.push_back(witch);
    }
    //---------------------------------------------------------------------------------
    // 座標を設定
    //---------------------------------------------------------------------------------
    //for文でループ
    for(int i = 0; i < characters.size(); i++) {
        //ロックしてstd::sharedを取る
        if(auto chara_shared = characters.at(i).lock()) {
            float most_left_x = -19.0f;    //一番左のキャラクターのx座標
            float deff_x      = 13.0f;     //キャラクターがずれていく大きさ
            float x           = most_left_x + deff_x * i;
            chara_shared->SetTranslate(float3(x, 0.0f, 0.0f));
        }
    }

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
        Scene::Change(Scene::GetScene<SceneCharaSelect>());    //シーンの変更を行う処理
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

    //---------------------------------------------------------------------------------
    //	各キャラクターの描画
    //---------------------------------------------------------------------------------
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
