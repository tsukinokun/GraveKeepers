//---------------------------------------------------------------------------
//!	@file	ComponentFireBall.cpp
//! @brief	ファイアボールのコンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentFireBall.h"
#include <Game/Scene/SkillObject/FireBall.h>
#include <System/Component/ComponentEffect.h>
#include <Game/System/SoundBuffer.h>

//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentFireBall::Init()
{    // 初期化処理
    __super::Init();
}

//---------------------------------------------------------------------------
//! @brief	更新処理関数
//---------------------------------------------------------------------------
void ComponentFireBall::Update()
{    // 初期化処理
    __super::Update();
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentFireBall::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"ファイアボールコンポーネント")) {
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

//---------------------------------------------------------------------------
//! @brief	スキルを発動する
//---------------------------------------------------------------------------
std::shared_ptr<ComponentSkill> ComponentFireBall::UseSkill()
{
    __super::UseSkill();

    //サウンドの再生
    int handle = SoundBuffer::GetSoundHandle("fire_ball");
    if(handle != -1) {
        PlaySoundMem(handle, DX_PLAYTYPE_BACK);
    }

    auto owner = GetOwner();
    //---------------------------------------------------------------------------
    // スキルオブジェクトの生成
    //---------------------------------------------------------------------------
    auto fire_ball  = Scene::Object::Create<FireBall>();    //ファイヤーボールオブジェクトを生成
    auto rotate     = owner->GetRotationAxisXYZ();
    rotate.y       += 180.0f;           //座標系の関係でyを180度回転する、オブジェクトの背中が正面
    fire_ball->SetDirection(rotate);    // 進行方向を設定

    fire_ball->SetSkillOwnerName(owner->GetName());    //スキル使用者の名前を設定(スキルが使用者に干渉しなくするために設定不可欠。)
    //高さの値は仮、後に持ち上げているオブジェクトの位置に変更するはず。
    const float3 pos = owner->GetTranslate() + float3(0.0f, 20.0f, 0.0f);
    fire_ball->SetTranslate(pos);

    fire_ball->SetScaleAxisXYZ(SCALE_);
    fire_ball->GetComponent<ComponentEffect>()->SetPlaySpeed(1.0f);
    //自身のポインタを変換
    return dynamic_pointer_cast<ComponentSkill>(shared_from_this());
}
CEREAL_REGISTER_TYPE(ComponentFireBall)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentFireBall)
