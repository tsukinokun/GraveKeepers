//---------------------------------------------------------------------------
//!	@file	Character.cpp
//! @brief	キャラクターのベースクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "Character.h"
#include <System/Component/ComponentObjectController.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentJump.h>
#include <System/Component/ComponentLift.h>
#include <System/Component/ComponentRigidbody.h>
#include <System/Component/ComponentLiftable.h>
#include <System/Component/ComponentStatus.h>
#include <System/State/StateIdleWalk.h>
#include "../../src/Game/system/HlslppUseful.h"
#include <System/Component/ComponentEffect.h>
#include <System/State/StateBase.h>
#include <System/State/StateKnockback.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Character::Init()
{
    __super::Init();
    SetStatus(Object::StatusBit::OnHitAllComponent, true);    //全てのコンポーネントに当たり判定を送る
    //---------------------------------------------------------------------------------
    // 剛体コンポーネント
    //---------------------------------------------------------------------------------
    rigidbody_component_ = AddComponent<ComponentRigidbody>();    //剛体コンポーネントを追加
    liftable_component_  = AddComponent<ComponentLiftable>();     //持ち上げられ機能コンポーネント
    auto hp_comp         = AddComponent<ComponentStatus>();       //HP機能コンポーネント
    status_component_    = hp_comp;
    //SetTranslate({0, 2, 0});
    //---------------------------------------------------------------------------------
    // コリジョン
    //---------------------------------------------------------------------------------
    auto col_comp = AddComponent<ComponentCollisionCapsule>();
    col_comp->SetRadius(RADIUS_);                                               // 球コリジョンの半径を2.0 にする
    col_comp->SetHeight(RADIUS_ + neutralpos_);                                 // 球コリジョンの高さを半径の４倍 にする
    col_comp->SetCollisionGroup(ComponentCollision::CollisionGroup::PLAYER);    // 所属するグループを「PLAYER」とします。
    collision_component_ = col_comp;
    auto jump_comp       = AddComponent<ComponentJump>();
    jump_component_      = jump_comp;
    auto model           = AddComponent<ComponentModel>();    //剛体コンポーネントを追加
    model->SetRotationAxisXYZ(float3(0.0f, 180.0f, 0.0f));    //モデルの回転を設定
    model_component_ = model;
    lift_component_  = AddComponent<ComponentLift>();    //持ち上げコンポーネント
    //---------------------------------------------------------------------------------
    // 状態コンポーネントをつける
    //---------------------------------------------------------------------------------
    AddComponent<StateIdleWalk>();
    SetName(u8"Character");

    //更新処理の後で、当った情報を解除
    auto post_update_proc = [this]() {
        damaged_by_thrown_object_ = false;    //ダメージを受けたフラグをリセット
    };
    SetProc("post_update_proc", post_update_proc, ProcTiming::LateUpdate, ProcPriority::NORMAL);

    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Character::Update()
{
    __super::Update();

    //座標が一定以下になったら初期スポーン位置に戻す
    if(IsRespawn()) {
        SetTranslate(float3(0.0f, 10.0f, 0.0f));
        if(auto rb = GetComponent<ComponentRigidbody>()) {
            rb->SetVelocity(float3(0.0f, 0.0f, 0.0f));
        }
    }

    if(auto hp = GetComponent<ComponentStatus>()) {
        //死亡で
        if(hp->IsDead()) {
            return;
        }
    }
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void Character::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void Character::Exit()
{
    __super::Exit();
}

//!GUI表示
void Character::GUI()
{
    __super::GUI();
}

void Character::OnHit(const ComponentCollision::HitInfo& hit_info)
{
    __super::OnHit(hit_info);
    auto hit_owner = hit_info.hit_collision_->GetOwner();

    //-------------------------------------------------------------------------
    // 1. 【自分が投げられた側】地面に激突してダメージを受ける処理
    //-------------------------------------------------------------------------
    if(auto my_liftable = liftable_component_.lock()) {
        // 投げられた状態(IsThrown)かつ、当たった相手が地面(Field)なら
        if(my_liftable->IsThrown() && hit_owner->GetNameDefault() == u8"Field") {
            if(auto my_rb = rigidbody_component_.lock()) {
                float speed = length(my_rb->GetVelocity());
                // 一定以上の速度でぶつかったら
                if(speed > 2.0f) {
                    if(auto my_status = status_component_.lock()) {
                        int g_damage = 10;    // 速度に応じた落下ダメージ
                        my_status->TakeDamage(g_damage);
                        // エフェクト再生
                        ComponentEffect::Object::Create("data/PoyPoy/Effect/Damage/hit.efkefc", hit_info.hit_position_);
                    }
                    // 激突したので投げフラグを下ろす
                    my_liftable->SetThrownFlag(false);
                }
            }
        }
    }

    //-------------------------------------------------------------------------
    // 2. 【相手が投げられた側】ぶつかってきた相手からダメージを受ける処理
    //-------------------------------------------------------------------------
    if(auto hit_liftable = hit_owner->GetComponent<ComponentLiftable>()) {
        // 投げられた状態（空中・飛行中）でなければ早期リターン
        if(!hit_liftable->IsThrown()) {
            return;
        }

        //投げられた対象が持ち上げられる状態であれば、早期リターン(持ち上げられる状態は一度地面に当たっている)
        if(hit_liftable->CanBeLifted()) {
            return;
        }

        // 持ち上げていたオーナーが自分自身なら、早期リターンする（自爆防止）
        if(auto lift_chara = hit_liftable->GetLiftCharacter()) {
            if(lift_chara->GetName() == GetName()) {
                return;
            }
        }

        // 剛体を取得
        if(auto hit_rb = hit_owner->GetComponent<ComponentRigidbody>()) {
            // 触ったオブジェクトの速度が少しでもあれば
            if(length(hit_rb->GetVelocity()) > float1(1.0f)) {
                float3 vel_dir   = normalize(hit_rb->GetVelocity());
                float3 pos_dir   = normalize(GetTranslate() - hit_owner->GetTranslate());
                float3 knock_dir = normalize(0.7f * vel_dir + 0.3f * pos_dir);
                float3 impulse   = knock_dir * length(hit_rb->GetVelocity());

                // 自分に衝撃を与える
                if(auto rb = GetComponent<ComponentRigidbody>()) {
                    rb->SetVelocity(float3(0.0f, 0.0f, 0.0f));
                    rb->AddImpulse(impulse);
                }

                // ダメージ計算
                int damage = 0;

                // キャンディー爆弾の場合は専用のダメージ値を使用
                if(hit_owner->GetName() == u8"キャンディー爆弾") {
                    damage = static_cast<int>(hit_rb->GetMass());    // 質量ベースのダメージ
                }
                else {
                    // それ以外は投げたキャラクターの攻撃力に基づく
                    if(auto lift_chara = hit_liftable->GetLiftCharacter()) {
                        if(auto lift_status = lift_chara->GetComponent<ComponentStatus>()) {
                            damage = lift_status->GetAttackPoints();
                        }
                    }
                }

                // --- 自分がダメージを受ける ---
                if(auto status_comp = status_component_.lock()) {
                    status_comp->TakeDamage(damage);
                }

                // --- 【追加】投げられた相手側もぶつかってダメージを受ける ---
                if(auto opponent_status = hit_owner->GetComponent<ComponentStatus>()) {
                    opponent_status->TakeDamage(damage);
                }

                // ダメージを受けたら
                if(damage > 0) {
                    //このタイミングで、このフレームにダメージを受けたフラグを立てる
                    damaged_by_thrown_object_ = true;
                    //エフェクトを再生
                    const std::string eff_name = "data/PoyPoy/Effect/Damage/hit.efkefc";
                    if(auto effect_obj = ComponentEffect::Object::Create(eff_name, hit_info.hit_position_)) {
                        if(auto effect_comp = effect_obj->GetComponent<ComponentEffect>()) {
                            effect_comp->SetPlaySpeed(1.0f);
                            effect_comp->SetScaleAxisXYZ(0.5f);
                        }
                    }
                }

                // 衝突したので相手の投げフラグを折る（多重ヒット防止）
                hit_liftable->SetThrownFlag(false);
            }
        }
    }
}

//---------------------------------------------------------------------------
//! @brief 生存しているかどうかを返す関数
//---------------------------------------------------------------------------
bool Character::IsAlive() const
{
    //ステータスコンポーネントが有効なら
    if(auto status = status_component_.lock()) {
        //生存しているかどうかを返す
        return !status->IsDead();
    }
    //ステータスコンポーネントが無効なら生存していないと返す
    return false;
}

//---------------------------------------------------------------------------
//! @brief 投げられたものでダメージを負ったかを返す関数
//---------------------------------------------------------------------------
bool Character::IsDamagedByThrownObject() const
{
    return damaged_by_thrown_object_;
}

//---------------------------------------------------------------------------
//! @brief リスポーンするかどうかを返す関数
//---------------------------------------------------------------------------
bool Character::IsRespawn()
{
    if(GetTranslate().y < -10.0f) {
        return true;
    }
    return false;
}
