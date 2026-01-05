//---------------------------------------------------------------------------
//! @file	SceneSkillSelect.cpp
//! @brief	スキル選択のシーン
//! @author 田中南々子
//---------------------------------------------------------------------------
#include "SceneSkillSelect.h"
#include "Camera.h"
#include "ScenePlay.h"
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentEffect.h>
#include <Game/Scene/UIObject/UIImage.h>
#include <Game/Scene/UIObject/UIText.h>
#include <Game/System/ImageBuffer.h>
#include <Game/System/GameRepository.h>

//---------------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------------
bool SceneSkillSelect::Init()
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
        camera->SetName("Camera");
    }
    std::vector<std::weak_ptr<Object>> skills;    // スキル格納用

    //---------------------------------------------------------------------------------
    // スペースキーを押して選択できる旨を表示
    //---------------------------------------------------------------------------------
    {
        auto ui_text = Scene::Object::Create<UIText>();
        ui_text->SetTranslate(float3(20.0f, 50.0f, 0.0f));
        ui_text->SetText("← → キーでスキルを選択、Spaceキーで決定");
        ui_text->SetFontSize(24);
        ui_text->SetName("SkillSelectText");
    }
    //---------------------------------------------------------------------------------
    // 本のモデルを表示
    //---------------------------------------------------------------------------------
    {
        auto model = Scene::Object::Create<Object>();
        model->SetTranslate(float3(0.0f, 0.0f, 0.0f));
        model->SetRotationAxisXYZ(float3(0.0f, 180, 0.0f));
        model->SetScaleAxisXYZ(float3(2.0f, 2.0f, 2.0f));
        model->SetName("BookModel");

        //---------------------------------------------------------------------------------
        //モデルコンポーネントの設定
        //---------------------------------------------------------------------------------
        auto model_comp = model->AddComponent<ComponentModel>();
        model_comp->Load("data/PoyPoy/Model//Book/book.mv1");
    }

    //---------------------------------------------------------------------------------
    // 各スキルを作成
    //---------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------
    // ファイアボール
    //---------------------------------------------------------------------------------
    {
        fireball_ = Scene::Object::Create<Object>();
        fireball_->SetTranslate(float3(0.0f, 10.0f, 0.0f));
        fireball_->SetName("FireBall");

        auto effect_comp = fireball_->AddComponent<ComponentEffect>();
        effect_comp->Load("data/PoyPoy/Effect/Fireball/FireBall.efkefc");

        skills.push_back(fireball_);
    }
    //---------------------------------------------------------------------------------
    // 突進
    //---------------------------------------------------------------------------------
    {
        dash_ = Scene::Object::Create<Object>();
        dash_->SetTranslate(float3(0.0f, 10.0f, 0.0f));
        dash_->SetName("Dash");

        auto effect_comp = dash_->AddComponent<ComponentEffect>();
        effect_comp->Load("data/PoyPoy/Effect/Dash/Simple_SpawnMethod1.efkefc");

        skills.push_back(dash_);
    }
    //---------------------------------------------------------------------------------
    // 毒設置
    //---------------------------------------------------------------------------------
    {
        poison_ = Scene::Object::Create<Object>();
        poison_->SetTranslate(float3(0.0f, 10.0f, 0.0f));
        poison_->SetName("Poison");

        auto effect_comp = poison_->AddComponent<ComponentEffect>();
        effect_comp->Load("data/PoyPoy/Effect/Poison/Poison.efkefc");

        skills.push_back(poison_);
    }
    //---------------------------------------------------------------------------------
    // 連撃
    //---------------------------------------------------------------------------------
    {
        combo_attack_ = Scene::Object::Create<Object>();
        combo_attack_->SetTranslate(float3(0.0f, 10.0f, 0.0f));
        combo_attack_->SetName("ComboAttack");

        auto effect_comp = combo_attack_->AddComponent<ComponentEffect>();
        effect_comp->Load("data/PoyPoy/Effect/ComboAttack/ComboAttack.efkefc");

        skills.push_back(combo_attack_);
    }
    manage_skills_ = skills;    // スキルオブジェクトを管理配列に格納

    return true;
}

//---------------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------------
void SceneSkillSelect::Update()
{
    __super::Update();
    //---------------------------------------------------------------------------------
    // 入力処理
    //---------------------------------------------------------------------------------
    //右キーで右へ
    if(IsKeyOn(KEY_INPUT_RIGHT)) {
        selected_skill_index_++;    // 次のスキルへ
        // インデックスが範囲外なら最初に戻す
        if(selected_skill_index_ >= manage_skills_.size()) {
            selected_skill_index_ = 0;
        }
    }
    //左キーで左へ
    if(IsKeyOn(KEY_INPUT_LEFT)) {
        selected_skill_index_--;    // 前のスキルへ
        // インデックスが範囲外なら最後に戻す
        if(selected_skill_index_ < 0) {
            selected_skill_index_ = static_cast<int>(manage_skills_.size()) - 1;
        }
    }
    //スキルの表示切り替え
    for(int i = 0; i < manage_skills_.size(); i++) {
        if(auto skill = manage_skills_[i].lock()) {
            if(i == selected_skill_index_) {
                if(auto effect_comp = skill->GetComponent<ComponentEffect>()) {
                    //再生されていなかったら
                    if(!effect_comp->IsPlaying()) {
                        effect_comp->Play(true);    // ループ再生
                    }
                }
            }
            else {
                if(auto effect_comp = skill->GetComponent<ComponentEffect>()) {
                    //再生されていたら
                    if(effect_comp->IsPlaying()) {
                        effect_comp->Stop();    // 再生を止める
                    }
                }
            }
        }
    }

    //SPACEキーが押されたらゲーム画面に移行
    if(IsKeyOn(KEY_INPUT_SPACE)) {
        if(auto skill = manage_skills_[selected_skill_index_].lock()) {
            // 選択されたスキルをセーブデータに保存
            GameRepository::Instance().SetSelectedSkillName(skill->GetName().data());
        }
        Scene::Change(Scene::GetScene<ScenePlay>());    //シーンの変更を行う処理
    }
}

//---------------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------------
void SceneSkillSelect::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//! 終了
//---------------------------------------------------------------------------------
void SceneSkillSelect::Exit()
{
    __super::Exit();
}

//!GUI表示
void SceneSkillSelect::GUI()
{
    __super::GUI();
    // ここにGUIの表示処理を追加
}
