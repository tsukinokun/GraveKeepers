//---------------------------------------------------------------------------
//!	@file	ComponentJump.cpp
//! @brief	ジャンプ機能コンポーネントの実装
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Component/ComponentJump.h>
#include <System/Component/ComponentRigidbody.h>
#include <System/Component/ComponentLiftable.h>
#include <System/Component/ComponentStatus.h>
#include <System/Component/ComponentHitInfo.h>

//---------------------------------------------------------------------------
//! @brief	初期化処理
//---------------------------------------------------------------------------
void ComponentJump::Init()
{
    __super::Init();
    //---------------------------------------------------------------------------
    // 更新処理登録
    //---------------------------------------------------------------------------
    auto update_proc = [this]() {
        is_jump_frame_ = false;         //ジャンプフレームではない状態へ
        auto owner     = GetOwner();    //オーナー取得
        //---------------------------------------------------------------------------
        // 死亡状態なら無視する
        //---------------------------------------------------------------------------
        if(auto status_comp = owner->GetComponent<ComponentStatus>()) {
            //死亡で
            if(status_comp->IsDead()) {
                //ジャンプ無効化
                return;
            }
        }
        //---------------------------------------------------------------------------
        // オーナーが持ち上げられ状態なら、後の処理は行わない。
        //---------------------------------------------------------------------------
        if(auto liftable_comp = owner->GetComponent<ComponentLiftable>()) {
            if(liftable_comp->IsLifted()) {
                return;
            }
        }
        //---------------------------------------------------------------------------
        // ジャンプ処理
        //---------------------------------------------------------------------------
        //外部のジャンプ条件を満たした際に
        if(conditions_jump_()) {
            //ジャンプ可能なら
            if(CanJump()) {
                //ジャンプ処理
                if(auto rb = owner->GetComponent<ComponentRigidbody>()) {
                    rb->AddImpulse(float3(0.0f, jump_force_, 0.0f));
                    is_jumping_    = true;
                    is_jump_frame_ = true;    //ジャンプフレームにする
                }
            }
        }
    };
    SetProc("Update", update_proc, ProcTiming::Update, ProcPriority::NONE);

    //---------------------------------------------------------------------------
    // ヒット時のコールバックを登録
    //---------------------------------------------------------------------------
    OnHitComponentFunc = [this](const HitInfo& hit_info) {
        auto hit_owner = hit_info.hit_collision_->GetOwner();
        //地面に当たったら
        if(hit_owner->GetNameDefault() == u8"Field") {
            is_jumping_ = false;    //ジャンプしていない状態へ
        }
    };
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
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

//---------------------------------------------------------------------------
//! @brief	ジャンプでどのくらい飛び上がるか
//---------------------------------------------------------------------------
void ComponentJump::SetJumpImpulse(float value)
{
    jump_force_ = value;
}

//---------------------------------------------------------------------------
//! @brief	有効無効をセット
//---------------------------------------------------------------------------
void ComponentJump::SetEnable(bool enable_flag)
{
    set_enable_ = enable_flag;
}

//---------------------------------------------------------------------------
//! @brief	ジャンプ中か否かを変換
//---------------------------------------------------------------------------
bool ComponentJump::IsJumping()
{
    return is_jumping_;
}

//---------------------------------------------------------------------------
//! @brief	ジャンプ条件を代入
//---------------------------------------------------------------------------
void ComponentJump::SetConditionsJump(std::function<bool()> condition)
{
    conditions_jump_ = condition;
}

//--------------------------------------------------------------------
//! @brief ジャンプしたフレームであることを返す関数
//--------------------------------------------------------------------
bool ComponentJump::IsJumpFrame()
{
    return is_jump_frame_;
}

//--------------------------------------------------------------------
//! @brief ジャンプ可能かを判定する関数
//--------------------------------------------------------------------
bool ComponentJump::CanJump()
{
    //--------------------------------------------------------------------
    // ジャンプが無効ならfalseを返す
    //--------------------------------------------------------------------
    if(!set_enable_) {
        return false;
    }
    //--------------------------------------------------------------------
    // ジャンプ中ならfalseを返す
    //--------------------------------------------------------------------
    if(is_jumping_) {
        return false;
    }
    //全ての条件を満たしているならtrueを返す
    return true;
}
CEREAL_REGISTER_TYPE(ComponentJump)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentJump)
