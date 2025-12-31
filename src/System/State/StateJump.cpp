//---------------------------------------------------------------------------
//!	@file	StateJump.cpp
//! @brief	ジャンプ状態コンポーネントの実装
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "StateJump.h"
#include <System/Component/ComponentSpringArm.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentStatus.h>
#include <System/State/StateDeath.h>
#include <System/State/StateIdleWalk.h>
#include <System/Component/ComponentLiftable.h>

//---------------------------------------------------------------------------
//! @brief 初期化処理
//---------------------------------------------------------------------------
void StateJump::Init()
{
    __super::Init();
    auto owner = GetOwner();    //オーナー取得
    //---------------------------------------------------------------------------
    // ジャンプアニメーションの単発再生
    //---------------------------------------------------------------------------
    if(auto model = owner->GetComponent<ComponentModel>()) {
        model->PlayAnimationNoSame("jump", false);
    }
    //---------------------------------------------------------------------------
    // 更新処理の登録
    //---------------------------------------------------------------------------
    auto update_proc = [this, owner]() {
        //---------------------------------------------------------------------------
        // 死亡で死亡状態へ遷移
        //---------------------------------------------------------------------------
        if(auto status = owner->GetComponent<ComponentStatus>()) {
            //死亡で
            if(status->IsDead()) {
                //死亡状態へ遷移
                ChangeState<StateDeath>();
            }
        }
        //---------------------------------------------------------------------------
        // ジャンプアニメーションが終了したら、待機状態へ
        //---------------------------------------------------------------------------
        if(auto model = owner->GetComponent<ComponentModel>()) {
            //アニメーションが終了したら
            if(!model->IsPlaying()) {
                //待機状態へ遷移
                ChangeState<StateIdleWalk>();
            }
        }
        //---------------------------------------------------------------------------
        // 持ち上げられ中なら、待機状態へ
        //---------------------------------------------------------------------------
        if(auto liftable_comp = owner->GetComponent<ComponentLiftable>()) {
            //持ち上げられ中なら
            if(liftable_comp->IsLifted()) {
                //待機状態へ遷移
                ChangeState<StateIdleWalk>();
            }
        }
    };
    SetProc("update_proc", update_proc, ProcTiming::Update, ProcPriority::NONE);
}

//---------------------------------------------------------------------------
//! @brief GUI表示
//---------------------------------------------------------------------------
void StateJump::GUI()
{
    __super::GUI();
    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"StateJump")) {
            // GUI上でオーナーから自分(SampleObjectController)を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

CEREAL_REGISTER_TYPE(StateJump)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, StateJump)
