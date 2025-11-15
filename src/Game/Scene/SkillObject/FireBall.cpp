//---------------------------------------------------------------------------
//!	@file	FireBall.cpp
//! @brief	ゲームシーンの火球オブジェクト
//! @auther 田中南々子
//---------------------------------------------------------------------------
#include "FireBall.h"
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentStatus.h>
#include <System/Component/ComponentRigidbody.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool FireBall::Init()
{
    __super::Init();

    SetName(u8"火球");
    SetEffect(u8"data/PoyPoy/Effect/FireBall/FireBall1.efkefc");
    //---------------------------------------------------------------------------------
    // コリジョンを追加する
    //---------------------------------------------------------------------------------
    auto col_comp = AddComponent<ComponentCollisionSphere>();
    col_comp->SetRadius(1.2f);
    col_comp->SetCollisionGroup(ComponentCollision::CollisionGroup::SKILL);    // 所属するグループを「SKILL」とします
    //PLAYERとGROUND、ETCに当たるように設定
    col_comp->SetHitCollisionGroup(static_cast<u32>(ComponentCollision::CollisionGroup::PLAYER) | static_cast<u32>(ComponentCollision::CollisionGroup::GROUND) |
                                   static_cast<u32>(ComponentCollision::CollisionGroup::ETC));

    col_comp->SetOverlapCollisionGroup(0xffffffff);    //すべてオーバーラップ(押し戻さないが、OnHitにコールバックは行うように)するように設定
    collision_component_ = col_comp;

    //リジッドボディを追加
    auto body = AddComponent<ComponentRigidbody>();
    body->SetMass(10.0f);    //質量を設定

    radius_ = 0.0f;

    return true;
}

void FireBall::Update()
{
    __super::Update();
    //当たったら消える
    if(is_hit_) {
        //エフェクトを切り替える
        if(effect_changed_ == false) {
            // エフェクト切替
            SetEffect(u8"data/PoyPoy/Effect/FireBall/FireBall2.efkefc");
            effect_changed_ = true;

            // 当たり後は物理力の付与など不要なので早期リターン
            hit_pos_ = GetTranslate();
            return;
        }

        //位置を当たった位置に固定する
        SetTranslate(hit_pos_);
    }
    else {
        //進行方向に力を加える
        //剛体を取得
        auto lift_rb = GetComponent<ComponentRigidbody>();
        //投げる力を計算
        float3 throw_impulse = float3(0.0f, throw_virtical_power_, 0.0f);
        //オーナーのy軸回転から、throw_impulse_のxとzを設定
        throw_impulse.x = -throw_horizontal_power_ * sinf(D2R(direction_.y));
        throw_impulse.z = -throw_horizontal_power_ * cosf(D2R(direction_.y));
        lift_rb->AddImpulse(throw_impulse);
        //コリジョンを有効化して重力を使うようにする
        auto lift_col = GetComponent<ComponentCollision>();
        lift_col->SetEnableFlag(true);
        lift_col->UseGravity();
    }
}

//---------------------------------------------------------------------------
//! @brief	進行方向を設定
//!	---------------------------------------------------------------------------
void FireBall::SetDirection(float3 direction)
{
    direction_ = direction;
}

//---------------------------------------------------------------------------
//! @brief	当たった情報はコールバックで送られてくる
//---------------------------------------------------------------------------
void FireBall::OnHit(const ComponentCollision::HitInfo& hit_info)
{
    __super::OnHit(hit_info);
    //キャラクターが当たったらダメージを受ける
    if(auto other_owner = hit_info.hit_collision_->GetOwner()) {
        //当たったオブジェクトの名前が使用者か確認
        if(other_owner->GetName() == skill_owner_name_) {
            //使用者なので何もしない
            return;
        }
        //ステータスコンポーネントを取得
        if(auto status_comp = other_owner->GetComponent<ComponentStatus>()) {
            //ダメージを与える(とりあえず4)
            status_comp->TakeDamage(ATTACK_DAMAGE_);
        }
        //当たったフラグを立てる
        is_hit_ = true;

        //コリジョンを膨らませる
        if(auto collision = collision_component_.lock()) {
            collision->SetRadius(RADUIS_ * 4.0f);
        }

        //ヒットした相手方の剛体に力を加える
        if(auto other = hit_info.hit_collision_->GetOwner()) {
            if(auto other_rb = other->GetComponent<ComponentRigidbody>()) {
                //自分から相手への方向ベクトルを計算
                float3 direction = other->GetTranslate() - GetTranslate();
                direction        = normalize(direction);
                //力を加える
                const float force_strength = 400.0f;
                other_rb->AddImpulse(direction * force_strength);
            }
        }
    }
}
