//---------------------------------------------------------------------------
//!	@file	Enemy.cpp
//! @brief	エネミー
//---------------------------------------------------------------------------
#include "Enemy.h"
#include <System/Component/ComponentObjectController.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentJump.h>
#include <System/Component/ComponentRigidbody.h>
#include <System/Component/ComponentLift.h>
#include <System/Component/ComponentLiftable.h>
#include <System/Component/ComponentHp.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Enemy::Init()
{
    __super::Init();
    AddComponent<ComponentRigidbody>();
    AddComponent<ComponentLiftable>();             //持ち上げられ機能コンポーネント
    auto hp_comp = AddComponent<ComponentHp>();    //HP機能コンポーネント
    hp_comp->SetHitPoints(HP_MAX_);
    SetTranslate({GetRand(PUT_RADIUS_MAX_) - PUT_RADIUS_MAX_ / 2, 2, GetRand(PUT_RADIUS_MAX_) - PUT_RADIUS_MAX_ / 2});
    auto col_comp = AddComponent<ComponentCollisionCapsule>();
    //col_comp->UseGravity();
    col_comp->SetRadius(RADIUS_);                   // 球コリジョンの半径を2.0 にする
    col_comp->SetHeight(RADIUS_ + neutral_pos_);    // 球コリジョンの高さを半径の４倍 にする
    auto jump_comp = AddComponent<ComponentJump>();

    //squat_timer_	 = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;	//AIができたら消してください
    //jump_timer_		 = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;	//AIができたら消してください
    //face_down_timer_ = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;	//AIができたら消してください
    //lift_timer_		 = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;	//AIができたら消してください
    //throw_timer_	 = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;	//AIができたら消してください

    jump_comp->SetConditionsJump([this]() {
        if(jump_timer_ < 0) {
            return true;
        }
        return false;
    });

    auto lift_comp = AddComponent<ComponentLift>();    //持ち上げコンポーネント
    lift_comp->SetConditionsForLifting(
        //ラムダ式を代入
        [this]() {
            if(set_lift_) {
                return true;
            }
            return false;
        });
    lift_comp->SetConditionsForThrow(    //ラムダ式を代入
        [this]() {
            if(throw_timer_ < 0) {
                return true;
            }
            return false;
        });

    SetName(u8"エネミー");

    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Enemy::Update()
{
    __super::Update();
    float3 rot = GetRotationAxisXYZ();
    //高さを半径の3倍にする
    neutral_pos_              = TOP_POINT_;
    float1 most_near_distance = std::numeric_limits<float>::max();    //とりあえず大きい数で初期化
    for(auto obj : Scene::Object::GetArray<Object>()) {
        if(obj->GetComponent<ComponentLiftable>() == nullptr) {
            continue;    //持ち上げられないオブジェクトはコンティニュー
        }
        if(obj->GetName() == GetName()) {
            continue;    //自分はコンティニュー
        }
        float3 owner_front = float3(0.0f, 0.0f, 0.0f);
        owner_front.x      = -1.0f * sinf(D2R(0.0f));
        owner_front.z      = -1.0f * cosf(D2R(0.0f));
        float3 owner_rot   = GetRotationAxisXYZ();
        //一応正規化
        owner_front = normalize(owner_front);
        //オブジェクトとオーナーのベクトルを取得
        float3 vec_owner_to_obj = obj->GetMatrix().translate() - GetMatrix().translate();
        //単位ベクトルを求める
        float3 normalize_vec = normalize(vec_owner_to_obj);
        //オブジェクトと持ち上げオーナーの内積を求める
        float obj_to_owner_dot = dot(owner_front.xz, normalize_vec.xz);
        //内積から角度を求める
        float rad = acosf(obj_to_owner_dot);

        //ベクトルの長さがこれまでに一番近かったオブジェクトよりも近いなら、監視対象オブジェクトを代入して、長さも代入する
        if(length(vec_owner_to_obj) < most_near_distance) {
            most_near_distance = length(vec_owner_to_obj);
            rot.y              = R2D(rad);
        }
    }
    SetRotationAxisXYZ(-rot);

    auto pos  = GetTranslate();
    pos.x    += MOVE_SPEED_ * sinf(D2R(rot.y));
    pos.z    -= MOVE_SPEED_ * cos(D2R(rot.y));

    SetTranslate(pos);

    //持ち上げられていない状態だったら
    if(!GetComponent<ComponentLiftable>()->IsLifted()) {
        //オブジェクトの位置が持ち上げられる範囲内にあったら
        if(most_near_distance < LIFT_RANGE_) {
            //持ち上げるかどうかを決めるフラグを立てる
            set_lift_ = true;
        }
        else {
            set_lift_ = false;
        }

        //-------ここに区切られているものはAI出来たら消してください-------------
        //squat_timer_--;
        //jump_timer_--;
        //face_down_timer_--;
        //if(lifting_block_ == false)
        //	lift_timer_--;
        //else
        //	throw_timer_--;
        //------------------------------------------------------------------
        //ジャンプをしていないなら
        if(GetComponent<ComponentJump>()->IsJumping() == false) {
            //ジャンプをできない状態にする
            GetComponent<ComponentJump>()->SetEnable();
            //高さを半径にする
            //neutral_pos_ = SQUAT_TOP_POINT_;
            //if(squat_timer_ < -RANDOM_TIME_MIN_)
            //{
            //	squat_timer_ = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;
            //}
        }
        else if(GetComponent<ComponentJump>()->IsJumping() == false) {
            //ジャンプをできない状態にする
            GetComponent<ComponentJump>()->SetEnable();
            //高さを半径にする
            //neutral_pos_ = FACE_DOWN_TOP_POINT_;
            //しゃがんでいると返す
            //if(face_down_timer_ < -RANDOM_TIME_MIN_)
            //{
            //	face_down_timer_ = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;
            //}
            is_face_down_ = true;
        }
        else {
            //しゃがんでいないと返す
            is_face_down_ = false;
        }

        //if(jump_timer_ < -RANDOM_TIME_MIN_)
        //{
        //	jump_timer_ = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;	   //AIができたら消してください
        //}
        //if(face_down_timer_ < -RANDOM_TIME_MIN_)
        //{
        //	face_down_timer_ = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;	//AIができたら消してください
        //}
        //if(lift_timer_ < -RANDOM_TIME_MIN_)
        //{
        //	lift_timer_	   = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;	  //AIができたら消してください
        //	lifting_block_ = true;
        //}
        //if(throw_timer_ < -RANDOM_TIME_MIN_)
        //{
        //	throw_timer_   = GetRand(RANDOM_TIME_MAX_) + RANDOM_TIME_MIN_;	  //AIができたら消してください
        //	lifting_block_ = false;
        //}
    }
    else {
        rot = float3(0, 0, 0);    //持ち上げられている状態なら回転をリセット
        //しゃがんでいないと返す
        is_face_down_ = false;
        SetRotationAxisXYZ(rot);    //回転をリセット
    }
    //コリジョンの高さの設定
    GetComponent<ComponentCollisionCapsule>()->SetHeight(RADIUS_ + neutral_pos_);
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void Enemy::Draw()
{
    float3 sphire_pos = float3(GetTranslate() + float3(0.0f, neutral_pos_, 0.0f));
    DrawSphere3D(cast(sphire_pos), RADIUS_, 16, WHITE, WHITE, TRUE);
    float3 cone_top    = float3(sphire_pos.xyz);
    float3 rot         = GetRotationAxisXYZ();
    float3 cone_bottom = float3(sphire_pos.x + (-5 * sinf(D2R(rot.y))), sphire_pos.y, sphire_pos.z + (-5 * cosf(D2R(rot.y))));
    DrawCone3D(cast(cone_bottom), cast(cone_top), RADIUS_, 16, WHITE, WHITE, TRUE);

    //うつ伏せの状態ではなかったら
    if(!is_face_down_) {
        //身体を表示する
        DrawCone3D(cast(sphire_pos), cast(GetTranslate()), RADIUS_, 16, WHITE, WHITE, TRUE);
    }

    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void Enemy::Exit()
{
    __super::Exit();
}

//!GUI表示
void Enemy::GUI()
{
    __super::GUI();
}

//---------------------------------------------------------------------------------
//!	ヒット時処理
//---------------------------------------------------------------------------------
void Enemy::OnHit(const ComponentCollision::HitInfo& hit_info)
{
    __super::OnHit(hit_info);
    auto hit_owner = hit_info.hit_collision_->GetOwner();
    if(auto hit_liftable = hit_owner->GetComponent<ComponentLiftable>()) {
        //持ち上げられ中(空中)でなければ
        if(!hit_liftable->IsLifted()) {
            return;    //早期リターン
        }
        //剛体を取得
        if(auto hit_rb = hit_owner->GetComponent<ComponentRigidbody>()) {
            //触ったオブジェクトの速度が少しでもあれば
            if(length(hit_rb->GetVelocity()) > float1(1.0f)) {
                GetComponent<ComponentRigidbody>()->AddImpulse(hit_rb->GetVelocity());    //ノックバック
                int damage = static_cast<int>(hit_rb->GetMass());                         //ダメージは当たったオブジェクトの質量に比例
                GetComponent<ComponentHp>()->TakeDamage(damage);                          //ダメージを受ける
            }
        }
    }
}
