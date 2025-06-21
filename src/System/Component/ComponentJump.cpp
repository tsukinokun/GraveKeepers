#pragma once
#include <System/Component/ComponentJump.h>

void ComponentJump::Init()
{
    __super::Init();
}

void ComponentJump::Update()
{
    __super::Update();
    jump_frame_count_--;
    auto owner = GetOwner();
    //スペースキー押下でジャンプ
    if(IsKeyOn(KEY_INPUT_SPACE) && (jump_frame_count_ < 0) && not_jump_mode_ == false) {
        jump_frame_count_ = jump_frame_max_;
        matrix mat        = owner->GetMatrix();
        float3 translate  = mat.translate();
        translate_hight_  = translate.y + jump_hight_;
        is_jump_mode_     = true;
    }
    //ジャンプのカウントが0以下ならリターン(この後の処理を行わない)
    if(jump_frame_count_ < 0) {
        not_jump_mode_ = false;
        is_jump_mode_  = false;
        return;
    }
    // オーナー(自分がAddComponentされたObject)を取得します
    // 処理されるときは必ずOwnerは存在しますので基本的にnullptrチェックは必要ありません
    matrix mat       = owner->GetMatrix();
    float3 translate = mat.translate();
    translate        = float3(translate.x, translate_hight_, translate.z);
    owner->SetTranslate(translate);
}

void ComponentJump::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"Jump")) {
            // GUI上でオーナーから自分(SampleObjectController)を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

//何フレームジャンプを行うかをセット
void ComponentJump::SetJumpFrame(int value)
{
    jump_frame_max_ = value;
}

//ジャンプでどのくらい飛び上がるか
void ComponentJump::SetJumpHight(float value)
{
    jump_hight_ = value;
}

void ComponentJump::Not_Jump()
{
    not_jump_mode_ = true;
}

bool ComponentJump::Is_Jump()
{
    return is_jump_mode_;
}
CEREAL_REGISTER_TYPE(ComponentJump)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentJump)
