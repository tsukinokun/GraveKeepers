//---------------------------------------------------------------------------
//!	@file	SceneCharaSelect.cpp
//! @brief	キャラセレクトのシーン
//---------------------------------------------------------------------------
#include "SceneCharaSelect.h"
#include "Camera.h"
#include <System/Component/ComponentModel.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool SceneCharaSelect::Init()
{
    __super::Init();
    //---------------------------------------------------------------------------------
    // カメラを作成
    //---------------------------------------------------------------------------------
    {
        auto camera   = Scene::Object::Create<Object>();    //カメラ
        auto cam_comp = camera->AddComponent<ComponentCamera>();
        //座標と注視点の設定
        cam_comp->SetPositionAndTarget({0, 20, 30}, {0, 10, 0});
    }
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
        zombie->SetName(u8"ゾンビ");
        //---------------------------------------------------------------------------------
        //モデルコンポーネントの設定
        //---------------------------------------------------------------------------------
        auto model_comp = zombie->AddComponent<ComponentModel>();
        model_comp->Load("data/PoyPoy/Model/Character/Zombie/Zombie.mv1");
        model_comp->SetAnimation({
            {"walk", "data/PoyPoy/Model/Character/Zombie/Anims/Walking.mv1", 0, 1.0f},
        });
        model_comp->PlayAnimation("walk", true);
    }
    //---------------------------------------------------------------------------------
    // 狼男
    //---------------------------------------------------------------------------------
    {
        auto wolf = Scene::Object::Create<Object>();
        wolf->SetTranslate(float3(0.0f, 0.0f, 0.0f));
        wolf->SetRotationAxisXYZ(float3(0.0f, 180.0f, 0.0f));
        wolf->SetName(u8"狼男");
        //---------------------------------------------------------------------------------
        //モデルコンポーネントの設定
        //---------------------------------------------------------------------------------
        auto model_comp = wolf->AddComponent<ComponentModel>();
        model_comp->Load("data/PoyPoy/Model/Character/Werewolf/Werewolf.mv1");
        model_comp->SetAnimation({
            {"walk", "data/PoyPoy/Model/Character/Werewolf/Anims/Orc Walk.mv1", 0, 1.0f},
        });
        model_comp->PlayAnimation("walk", true);
    }
    //---------------------------------------------------------------------------------
    // パンプキング
    //---------------------------------------------------------------------------------
    {
        auto pumpkin = Scene::Object::Create<Object>();
        pumpkin->SetTranslate(float3(0.0f, 0.0f, 0.0f));
        pumpkin->SetRotationAxisXYZ(float3(0.0f, 180.0f, 0.0f));
        pumpkin->SetName(u8"カボチャ野郎");
        //---------------------------------------------------------------------------------
        //モデルコンポーネントの設定
        //---------------------------------------------------------------------------------
        auto model_comp = pumpkin->AddComponent<ComponentModel>();
        model_comp->Load("data/PoyPoy/Model/Character/Pumpking/Pumpking.mv1");
        model_comp->SetAnimation({
            {"walk", "data/PoyPoy/Model/Character/Pumpking/Anims/Running.mv1", 1, 1.0f},
        });
        model_comp->PlayAnimation("walk", true);
    }
    //---------------------------------------------------------------------------------
    // ウィッチ
    //---------------------------------------------------------------------------------
    {
        auto witch = Scene::Object::Create<Object>();
        witch->SetTranslate(float3(0.0f, 0.0f, 0.0f));
        witch->SetRotationAxisXYZ(float3(0.0f, 180.0f, 0.0f));
        witch->SetName(u8"魔女っ子");
        //---------------------------------------------------------------------------------
        //モデルコンポーネントの設定
        //---------------------------------------------------------------------------------
        auto model_comp = witch->AddComponent<ComponentModel>();
        model_comp->Load("data/PoyPoy/Model/Character/Witch/Witch.mv1");
        model_comp->SetAnimation({
            {"walk", "data/PoyPoy/Model/Character/Witch/Anims/Walking.mv1", 0, 1.0f},
        });
        model_comp->PlayAnimation("walk", true);
    }
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
