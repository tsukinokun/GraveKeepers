//---------------------------------------------------------------------------
//!	@file	Enemy.cpp
//! @brief	エネミー
//---------------------------------------------------------------------------
#include "Enemy.h"
#include <Game/Scene/Character/Zombie/Zombie.h>
#include <System/Component/ComponentObjectController.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentJump.h>
#include <System/Component/ComponentRigidbody.h>
#include <System/Component/ComponentLift.h>
#include <System/Component/ComponentLiftable.h>
#include <System/Component/ComponentStatus.h>
#include <System/Component/ComponentAI.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Enemy::Init()
{
    __super::Init();
    auto chara = Scene::Object::Create<Zombie>();    //テスト、プレイヤーでゾンビを作成、後々選択したものに変更する。
    //auto chara		   = Scene::Object::Create<Werewolf>();	   //狼男を表示するためにゾンビをコメントアウトしています
    //auto chara		   = Scene::Object::Create<Pumpking>();
    chara->AddComponent<ComponentAI>();
    if(auto jump_comp = chara->GetComponent<ComponentJump>()) {
        jump_comp->SetConditionsJump([]() { return false; });
    }

    if(auto lift_comp = chara->GetComponent<ComponentLift>()) {
        lift_comp->SetConditionsForLifting(
            //ラムダ式を代入
            []() { return true; });
        lift_comp->SetConditionsForThrow(    //ラムダ式を代入
            [this]() {
                if(auto controll_lock = controll_character_.lock()) {
                    if(auto ai = controll_lock->GetComponent<ComponentAI>()) {
                        return ai->ThrowSignal();
                    }
                }
                return false;
            });
    }
    controll_character_ = chara;
    SetName(u8"プレイヤー");
    //AddComponent<ComponentRigidbody>();
    //AddComponent<ComponentLiftable>();				   //持ち上げられ機能コンポーネント
    //auto hp_comp = AddComponent<ComponentStatus>();	   //HP機能コンポーネント
    //hp_comp->SetHitPoints(HP_MAX_);
    //SetTranslate({GetRand(PUT_RADIUS_MAX_) - PUT_RADIUS_MAX_ / 2, 2, GetRand(PUT_RADIUS_MAX_) - PUT_RADIUS_MAX_ / 2});
    //auto col_comp = AddComponent<ComponentCollisionCapsule>();
    ////col_comp->UseGravity();
    //col_comp->SetRadius(RADIUS_);					// 球コリジョンの半径を2.0 にする
    //col_comp->SetHeight(RADIUS_ + neutral_pos_);	// 球コリジョンの高さを半径の４倍 にする
    //auto jump_comp = AddComponent<ComponentJump>();
    //SetRotationAxisXYZ({0.0f, GetRand(360), 0.0f});
    //jump_comp->SetConditionsJump(
    //	[this]()
    //	{
    //		if(set_jump_)
    //		{
    //			return true;
    //		}
    //		return false;
    //	});

    //auto lift_comp = AddComponent<ComponentLift>();	   //持ち上げコンポーネント
    //lift_comp->SetConditionsForLifting(
    //	//ラムダ式を代入
    //	[this]()
    //	{
    //		if(set_lift_ == true)
    //		{
    //			return true;
    //		}
    //		return false;
    //	});
    //lift_comp->SetConditionsForThrow(	 //ラムダ式を代入
    //	[this]()
    //	{
    //		if(set_throw_ == true)
    //		{
    //			set_lift_  = false;	   //持ち上げフラグを下ろす
    //			set_throw_ = false;
    //			return true;
    //		}
    //		return false;
    //	});

    //previous_time_ = std::chrono::high_resolution_clock::now();

    SetName(u8"エネミー");

    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Enemy::Update()
{
    __super::Update();

    //float3 rot				  = GetRotationAxisXYZ();

    //float1 most_near_distance = std::numeric_limits<float>::max();	  //とりあえず大きい数で初期化

    //if(auto hp = GetComponent<ComponentStatus>())
    //{
    //	//死亡で
    //	if(hp->IsDead())
    //	{
    //		return;
    //	}
    //}

    ////持ち上げられていない状態だったら
    //if(!GetComponent<ComponentLiftable>()->IsLifted())
    //{
    //	auto current_time = std::chrono::high_resolution_clock::now();
    //	auto delta_time	  = current_time - previous_time_;
    //	previous_time_	  = current_time;

    //	// タイマーを減算（カウントダウン）
    //	timer_count_ -= std::chrono::duration<float>(delta_time).count();
    //	int seconds	  = static_cast<int>(timer_count_) % 60;

    //	if(set_lift_ == false)
    //	{
    //		for(auto obj : Scene::Object::GetArray<Object>())
    //		{
    //			if(obj->GetComponent<ComponentLiftable>() == nullptr)
    //			{
    //				continue;	 //持ち上げられないオブジェクトはコンティニュー
    //			}
    //			if(obj->GetName() == GetName())
    //			{
    //				continue;	 //自分はコンティニュー
    //			}
    //			if(GetComponent<ComponentLiftable>()->IsLifted())
    //			{
    //				continue;	 //オーナーが持ち上げられ中なら持ち上げない
    //			}
    //			if(auto obj_lif_comp = obj->GetComponent<ComponentLift>())
    //			{
    //				if(obj_lif_comp->IsLifting())	 //オブジェクトが持ち上げ中ならコンテニュー
    //				{
    //					continue;
    //				}
    //			}

    //			float3 owner_front = float3(0.0f, 0.0f, 0.0f);
    //			owner_front.x	   = -1.0f * sinf(D2R(0.0f));
    //			owner_front.z	   = -1.0f * cosf(D2R(0.0f));
    //			float3 owner_rot   = GetRotationAxisXYZ();
    //			//一応正規化
    //			owner_front = normalize(owner_front);
    //			//オブジェクトとオーナーのベクトルを取得
    //			float3 vec_owner_to_obj = obj->GetMatrix().translate() - GetMatrix().translate();
    //			//単位ベクトルを求める
    //			float3 normalize_vec = normalize(vec_owner_to_obj);
    //			//オブジェクトと持ち上げオーナーの内積を求める
    //			float obj_to_owner_dot = dot(owner_front.xz, normalize_vec.xz);
    //			//内積から角度を求める
    //			float rad = acosf(obj_to_owner_dot);
    //			//ベクトルの長さがこれまでに一番近かったオブジェクトよりも近いなら、監視対象オブジェクトを代入して、長さも代入する
    //			if(length(vec_owner_to_obj) < most_near_distance)
    //			{
    //				most_near_distance = length(vec_owner_to_obj);
    //				rot.y			   = R2D(rad);
    //			}
    //		}
    //		SetRotationAxisXYZ(-rot);
    //	}

    //	//オブジェクトの位置が持ち上げられる範囲内にあったら
    //	if(most_near_distance < LIFT_RANGE_ && set_lift_ == false)
    //	{
    //		//持ち上げるかどうかを決めるフラグを立てる
    //		set_lift_ = true;
    //	}

    //	if(seconds <= 0)
    //	{
    //		is_face_down_ = false;
    //		//ジャンプかしゃがみかうつ伏せをするかを決める
    //		int state_rand = GetRand(2);
    //		if(state_rand == 0)
    //		{
    //			//高さを半径の3倍にする
    //			neutral_pos_ = TOP_POINT_;

    //			set_jump_	 = true;	//ジャンプするかどうかを決めるフラグを立てる
    //		}
    //		//ジャンプをしていないなら
    //		else if(state_rand == 1 && GetComponent<ComponentJump>()->IsJumping() == false)
    //		{
    //			//ジャンプをできない状態にする
    //			GetComponent<ComponentJump>()->SetEnable();
    //			//高さを半径にする
    //			neutral_pos_ = SQUAT_TOP_POINT_;
    //		}
    //		else if(state_rand == 2 && GetComponent<ComponentJump>()->IsJumping() == false)
    //		{
    //			//ジャンプをできない状態にする
    //			GetComponent<ComponentJump>()->SetEnable();
    //			//高さを半径にする
    //			neutral_pos_ = FACE_DOWN_TOP_POINT_;
    //			//しゃがんでいると返す
    //			is_face_down_ = true;
    //		}

    //		timer_count_ = static_cast<float>(GetRand(TIMER_RANDOM_MAX_) + 1);	  //タイマーをリセット
    //	}
    //	else
    //	{
    //		set_jump_ = false;	  //ジャンプするかどうかを決めるフラグを下ろす
    //	}
    //}
    //else
    //{
    //	//しゃがんでいないと返す
    //	is_face_down_ = false;
    //}
    //if(set_lift_ == true)
    //{
    //	for(auto obj : Scene::Object::GetArray<Object>())
    //	{
    //		if(obj->GetComponent<ComponentLiftable>() == nullptr)
    //		{
    //			continue;	 //持ち上げられないオブジェクトはコンティニュー
    //		}
    //		if(obj->GetName() == GetName())
    //		{
    //			continue;	 //自分はコンティニュー
    //		}
    //		if(obj->GetComponent<ComponentLiftable>()->IsLifted())
    //		{
    //			continue;	 //オーナーが持ち上げられ中なら持ち上げない
    //		}

    //		float3 owner_front = float3(0.0f, 0.0f, 0.0f);
    //		owner_front.x	   = -1.0f * sinf(D2R(0.0f));
    //		owner_front.z	   = -1.0f * cosf(D2R(0.0f));
    //		float3 owner_rot   = GetRotationAxisXYZ();
    //		//一応正規化
    //		owner_front = normalize(owner_front);
    //		//オブジェクトとオーナーのベクトルを取得
    //		float3 vec_owner_to_obj = obj->GetMatrix().translate() - GetMatrix().translate();
    //		//単位ベクトルを求める
    //		float3 normalize_vec = normalize(vec_owner_to_obj);
    //		//オブジェクトと持ち上げオーナーの内積を求める
    //		float obj_to_owner_dot = dot(owner_front.xz, normalize_vec.xz);
    //		//内積から角度を求める
    //		float rad = acosf(obj_to_owner_dot);
    //		//ベクトルの長さがこれまでに一番近かったオブジェクトよりも近いなら、監視対象オブジェクトを代入して、長さも代入する
    //		if(length(vec_owner_to_obj) < most_near_distance)
    //		{
    //			if(obj->GetName() == u8"エネミー" && obj->GetName() != GetName() || obj->GetName() == u8"プレイヤー")
    //			{
    //				most_near_distance = length(vec_owner_to_obj);
    //				rot.y			   = R2D(rad);
    //			}
    //			else
    //				continue;
    //		}
    //	}
    //	SetRotationAxisXYZ(-rot);

    //	if(most_near_distance < THROW_RANGE_)
    //	{
    //		set_throw_ = true;
    //	}
    //}

    //auto pos  = GetTranslate();
    //pos.x	 += MOVE_SPEED_ * sinf(D2R(rot.y));
    //pos.z	 -= MOVE_SPEED_ * cos(D2R(rot.y));

    //SetTranslate(pos);

    //コリジョンの高さの設定
    //GetComponent<ComponentCollisionCapsule>()->SetHeight(RADIUS_ + neutral_pos_);
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void Enemy::Draw()
{
    //float3 sphire_pos = float3(GetTranslate() + float3(0.0f, neutral_pos_, 0.0f));
    //DrawSphere3D(cast(sphire_pos), RADIUS_, 16, WHITE, WHITE, TRUE);
    //float3 cone_top	   = float3(sphire_pos.xyz);
    //float3 rot		   = GetRotationAxisXYZ();
    //float3 cone_bottom = float3(sphire_pos.x + (-5 * sinf(D2R(rot.y))), sphire_pos.y, sphire_pos.z + (-5 * cosf(D2R(rot.y))));
    //DrawCone3D(cast(cone_bottom), cast(cone_top), RADIUS_, 16, WHITE, WHITE, TRUE);

    ////うつ伏せの状態ではなかったら
    //if(!is_face_down_)
    //{
    //	//身体を表示する
    //	DrawCone3D(cast(sphire_pos), cast(GetTranslate()), RADIUS_, 16, WHITE, WHITE, TRUE);
    //}

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
    //__super::OnHit(hit_info);
    //auto hit_owner = hit_info.hit_collision_->GetOwner();
    //if(auto hit_liftable = hit_owner->GetComponent<ComponentLiftable>())
    //{
    //	//持ち上げられ中(空中)でなければ
    //	if(!hit_liftable->IsLifted())
    //	{
    //		return;	   //早期リターン
    //	}
    //	//剛体を取得
    //	if(auto hit_rb = hit_owner->GetComponent<ComponentRigidbody>())
    //	{
    //		//触ったオブジェクトの速度が少しでもあれば
    //		if(length(hit_rb->GetVelocity()) > float1(1.0f))
    //		{
    //			GetComponent<ComponentRigidbody>()->AddImpulse(hit_rb->GetVelocity());	  //ノックバック
    //			int damage = static_cast<int>(hit_rb->GetMass());						  //ダメージは当たったオブジェクトの質量に比例
    //			GetComponent<ComponentStatus>()->TakeDamage(damage);					  //ダメージを受ける
    //		}
    //	}
    //}
}
