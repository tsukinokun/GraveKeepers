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
#include <Game/Scene/UIObject/UIGauge.h>
#include "../../src/Game/system/HlslppUseful.h"
#include <System/Component/ComponentEffect.h>

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
    SetTranslate({0, 2, 0});
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
    //---------------------------------------------------------------------------------
    // オブジェクトの頭の上にゲージを表示させる
    //---------------------------------------------------------------------------------
    {
        auto gauge       = Scene::Object::Create<UIGauge>();
        auto update_proc = [gauge, this]() {
            //---------------------------------------------------------------------------------
            // ゲージの位置をキャラクターの頭の上に設定する処理
            //---------------------------------------------------------------------------------
            //ワールド空間スクリーン空間に変換したい
            if(auto camera = Scene::GetCurrentCamera().lock()) {
                gauge->SetTranslate(float3(WorldPositionToScreenPosition(GetTranslate()), 0.0f));    //ゲージの位置を設定
            }
            //---------------------------------------------------------------------------------
            // ゲージの割合を設定する処理
            //---------------------------------------------------------------------------------
            if(auto status = GetComponent<ComponentStatus>()) {
                float hp_rate = static_cast<float>(status->GetHitPoints()) / static_cast<float>(status->GetMaxHitPoints());
                gauge->SetGaugeRate(hp_rate);    //ゲージの割合を設定
            }
        };
        gauge->SetProc("update", update_proc);
    }
    SetName(u8"Character");

    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Character::Update()
{
    __super::Update();
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
    if(auto hit_liftable = hit_owner->GetComponent<ComponentLiftable>()) {
        //持ち上げられ中(空中)でなければ
        if(!hit_liftable->IsLifted()) {
            return;    //早期リターン
        }
        //持ち上げていたオーナーが自分自身なら、早期リターンする
        if(auto lift_chara = hit_liftable->GetLiftCharacter()) {
            if(lift_chara->GetName() == GetName()) {
                return;    //早期リターン
            }
        }
        //剛体を取得
        if(auto hit_rb = hit_owner->GetComponent<ComponentRigidbody>()) {
            //触ったオブジェクトの速度が少しでもあれば
            if(length(hit_rb->GetVelocity()) > float1(1.0f)) {
                float3 vel_dir   = normalize(hit_rb->GetVelocity());
                float3 pos_dir   = normalize(GetTranslate() - hit_owner->GetTranslate());
                float3 knock_dir = normalize(0.7f * vel_dir + 0.3f * pos_dir);
                float3 impulse   = knock_dir * length(hit_rb->GetVelocity());
                //衝撃を与える
                if(auto rb = GetComponent<ComponentRigidbody>()) {
                    rb->SetVelocity(float3(0.0f, 0.0f, 0.0f));
                    rb->AddImpulse(impulse);
                }

                //GetComponent<ComponentRigidbody>()->AddImpulse(hit_rb->GetVelocity());
                int damage = static_cast<int>(hit_rb->GetMass());       //ダメージは当たったオブジェクトの質量に比例
                GetComponent<ComponentStatus>()->TakeDamage(damage);    //ダメージを受ける

                float3 hit_pos = hit_info.hit_position_;
                // ダメージを受けたらエフェクトを再生（ダメージが0より大きい場合のみ）
                if(damage > 0) {
                    // 既存で使われているエフェクトを流用。必要ならパスを変更してください。
                    const std::string eff_name = "data/PoyPoy/Effect/Damage/hit.efkefc";
                    float3            pos      = hit_pos;
                    // エフェクト生成
                    auto effectObj = ComponentEffect::Object::Create(eff_name, pos);

                    // 再生速度を2倍にする, スケールを変更
                    if(effectObj) {
                        effectObj->GetComponent<ComponentEffect>()->SetPlaySpeed(1.0f);
                        effectObj->GetComponent<ComponentEffect>()->SetScaleAxisXYZ(0.5f);
                    }
                }
            }
        }
    }
}

//---------------------------------------------------------------------------
// 生存しているかどうかを返す関数
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
