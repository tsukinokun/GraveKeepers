//---------------------------------------------------------------------------
//!	@file	SceneCharaSelect.cpp
//! @brief	キャラセレクトのシーン
//---------------------------------------------------------------------------
#include "SceneCharaSelect.h"
#include "Camera.h"
#include <System/Component/ComponentModel.h>
#include <Game/System/HlslppUseful.h>
#include <Game/Scene/UIObject/UIText.h>
#include <Game/Scene/ScenePlay.h>
#include <Game/System/GameRepository.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool SceneCharaSelect::Init()
{
    __super::Init();
    //---------------------------------------------------------------------------------
    // 背景画像の読み込み
    //---------------------------------------------------------------------------------
    image_handle = LoadGraph("data/PoyPoy/Image/Select.png");

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
    // キャラクターを円形に配置
    //---------------------------------------------------------------------------------
    for(int i = 0; i < characters.size(); i++) {
        float3 center    = float3(0.0f, 0.0f, 0.0f);    // 円の中心
        float  radius    = 10.0f;                       // 円の半径
        float3 translate = GetPointOnCircle(center, radius, characters.size(), i, rad_display_offset_);
        if(auto chara = characters[i].lock()) {
            chara->SetTranslate(translate);
        }
    }
    manage_characters_ = characters;    // 管理用に保存
    //---------------------------------------------------------------------------------
    // エンターキーを押して選択できる旨を表示
    //---------------------------------------------------------------------------------
    {
        auto ui_text = Scene::Object::Create<UIText>();
        ui_text->SetTranslate(float3(20.0f, 50.0f, 0.0f));
        ui_text->SetText("← → キーでキャラクターを選択、Spaceキーで決定");
        ui_text->SetFontSize(24);
    }
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void SceneCharaSelect::Update()
{
    __super::Update();
    //---------------------------------------------------------------------------------
    // 入力処理
    //---------------------------------------------------------------------------------
    //右キーで右へ
    if(IsKeyOn(KEY_INPUT_RIGHT)) {
        selected_character_index_++;    // 次のキャラクターへ
        // インデックスが範囲外なら最初に戻す
        if(selected_character_index_ >= manage_characters_.size()) {
            selected_character_index_ = 0;
        }
        // 角度のオフセットを更新
        rad_offset_ -= 360.0f / static_cast<float>(manage_characters_.size());
    }
    //左キーで左へ
    if(IsKeyOn(KEY_INPUT_LEFT)) {
        selected_character_index_--;    // 前のキャラクターへ
        // インデックスが範囲外なら最後に戻す
        if(selected_character_index_ < 0) {
            selected_character_index_ = static_cast<int>(manage_characters_.size()) - 1;
        }
        // 角度のオフセットを更新
        rad_offset_ += 360.0f / static_cast<float>(manage_characters_.size());
    }
    //角度を滑らかに補間
    rad_display_offset_ += (rad_offset_ - rad_display_offset_) * 0.1f;
    //---------------------------------------------------------------------------------
    // キャラクターを円形に配置
    //---------------------------------------------------------------------------------
    for(int i = 0; i < manage_characters_.size(); i++) {
        float3 center    = float3(0.0f, 0.0f, 0.0f);    // 円の中心
        float  radius    = 10.0f;                       // 円の半径
        float3 translate = GetPointOnCircle(center, radius, manage_characters_.size(), i, rad_display_offset_);
        if(auto chara = manage_characters_[i].lock()) {
            chara->SetTranslate(translate);
        }
    }
    //---------------------------------------------------------------------------------
    // エンターキーで決定してゲーム開始
    //---------------------------------------------------------------------------------
    if(IsKeyOn(KEY_INPUT_SPACE)) {
        // 選択されたキャラクター名をリポジトリに保存
        if(auto selected_chara = manage_characters_[selected_character_index_].lock()) {
            GameRepository::Instance().SetSelectedCharacterName(selected_chara->GetNameDefault().data());
        }
        Scene::Change(Scene::GetScene<ScenePlay>());    //シーンの変更を行う処理
    }
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void SceneCharaSelect::Draw()
{
    __super::Draw();

    //  背景画像の描画
    if(image_handle != -1) {
        // 画面全体に描画
        DrawExtendGraph(0, 0, WINDOW_W, WINDOW_H, image_handle, TRUE);
    }
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void SceneCharaSelect::Exit()
{
    __super::Exit();
    // ここにゲームの終了処理を追加

    // 背景画像の削除
    if(image_handle != -1) {
        DeleteGraph(image_handle);
        image_handle = -1;
    }
}

//!GUI表示
void SceneCharaSelect::GUI()
{
    __super::GUI();
    // ここにGUIの表示処理を追加
}
