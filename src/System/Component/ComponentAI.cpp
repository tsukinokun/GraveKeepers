//---------------------------------------------------------------------------
//!	@file	ComponentAI.cpp
//! @brief	AIコンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Component/ComponentAI.h>
#include <System/Component/ComponentRigidbody.h>
#include <System/Component/ComponentLiftable.h>
#include <System/Component/ComponentLift.h>
#include <System/Component/ComponentStatus.h>
#include <System/State/StateDeath.h>
#include <System/State/StateKnockback.h>
#include <Game/Scene/Character/Base/Character.h>

//---------------------------------------------------------------------------
//! @brief	初期化処理
//---------------------------------------------------------------------------
void ComponentAI::Init()
{
    __super::Init();
}

//---------------------------------------------------------------------------
//! @brief	更新処理
//---------------------------------------------------------------------------
void ComponentAI::Update()
{
    __super::Update();
    //時間の更新
    current_time_    = std::chrono::high_resolution_clock::now();
    float delta_time = std::chrono::duration<float>(current_time_ - prev_time_).count();
    prev_time_       = current_time_;
    auto owner       = GetOwner();    //オーナーを取得
    //オーナーが死亡かノックバック状態ならこれ以降の処理を行わない
    if(owner->GetComponent<StateDeath>() && owner->GetComponent<StateKnockback>()) {
        return;
    }
    //持ち上げられないオブジェクトの場合はこれ以降の処理を行わない
    if(auto liftable_comp = owner->GetComponent<ComponentLiftable>()) {
        if(!liftable_comp->CanBeLifted()) {
            //持ち上げられない状態ならこれ以降の処理を行わない
            return;
        }
    }
    float3 rot  = owner->GetRotationAxisXYZ();
    rot.y      += 180.0f;    //座標系の違いの関係で180度回転させる

    //持ち上げコンポーネント処理
    if(auto lift_comp = owner->GetComponent<ComponentLift>()) {
        //持ち上げたフレームの処理
        if(lift_comp->IsJustLifted()) {
            auto                                    chara_array = Scene::Object::GetArray<Character>();
            std::vector<std::shared_ptr<Character>> alive_chara_vec;
            for(const auto& chara : chara_array) {
                if(!chara->GetComponent<StateDeath>()) {
                    //生きているならベクターに追加
                    alive_chara_vec.push_back(chara);
                }
            }
            std::random_device                 rd;                                     // 疑似乱数のソース
            std::mt19937                       mt(rd());                               // メルセンヌ・ツイスタの宣言と初期化
            std::uniform_int_distribution<int> dist(0, alive_chara_vec.size() - 1);    // 1から100で整数の一様分布を作る
            int                                index = dist(mt);                       // 乱数を生成
            target_object_                           = alive_chara_vec[index];         //ターゲットオブジェクトをプレイヤーに設定
        }
        float1 most_near_distance = std::numeric_limits<float>::max();    //とりあえず大きい数で初期化
        float3 most_near_vec      = float3(0.0f, 0.0f, 0.0f);             //一番近いオブジェクトのベクトル
        //持ち上げ中でない場合の処理
        if(!lift_comp->IsLifting()) {
            lift_time_count_ = 0.0f;    //持ち上げ中でないので、0にする

            //オブジェクトを取得
            for(auto obj : Scene::Object::GetArray<Object>()) {
                if(auto liftable_comp = obj->GetComponent<ComponentLiftable>()) {
                    //現在持ち上げられているオブジェクトの場合はコンティニュー
                    if(liftable_comp->IsLifted()) {
                        continue;
                    }
                }
                else {
                    continue;    //そもそも持ち上げられないオブジェクトはコンティニュー
                }
                if(obj->GetComponent<ComponentLift>()) {
                    continue;    //操作オブジェクト以外はコンティニュー
                }
                auto obj_name = obj->GetName();
                if(owner->GetName() == obj->GetName()) {
                    continue;    //自分はコンティニュー
                }
                //オブジェクトとオーナーのベクトルを取得
                float3 vec_owner_to_obj = obj->GetTranslate() - owner->GetTranslate();
                float1 distance         = length(vec_owner_to_obj);
                if(obj->GetNameDefault() == u8"キャンディー爆弾") {
                    distance - 20.0f;    //キャンディー爆弾は少し補正をかける(遠くても拾いに行く)
                }
                //ベクトルの長さがこれまでに一番近かったオブジェクトよりも近いなら、長さとそのベクトルを代入する
                if(distance < most_near_distance) {
                    most_near_distance = length(vec_owner_to_obj);
                    most_near_vec      = vec_owner_to_obj;
                }
            }
        }
        else {
            //持ち上げ中の処理
            lift_time_count_ += delta_time;    //持ち上げ中なので、デルタタイムを加算
            //オブジェクトとオーナーのベクトルを取得
            if(auto target = target_object_.lock()) {
                float3 vec_owner_to_obj = target->GetTranslate() - owner->GetTranslate();
                most_near_distance      = length(vec_owner_to_obj);
                most_near_vec           = vec_owner_to_obj;
            }
        }
        //----------------------------------------------------------------------------------
        // キャラクターの移動(最も近いオブジェクトに近づく)
        //----------------------------------------------------------------------------------
        auto   world_mat = owner->Matrix();
        float3 position  = world_mat[3].xyz;               //ワールド座標の一成分を取得
        float3 front     = normalize(world_mat[2].xyz);    //前方の単位ベクトルを取得
        float3 up        = normalize(world_mat[1].xyz);    //上方の単位ベクトルを取得
        float3 right     = normalize(world_mat[0].xyz);    //右の単位ベクトルを取得
        //移動方向は、最も近いオブジェクトとの方向ベクトル
        float3 move = most_near_vec;
        move.y      = 0.0f;    //y成分は0にする(上下移動しない)
        // 調整
        if(float1(0.0001f) < dot(move, move)) {
            move = normalize(move);    //正規化
            dir_ = move;               // 内部の方向を更新
        }
        //移動
        position += move * move_speed_;

        float cosine = dot(display_dir_, dir_);    //ベクトルのなす角
        cosine       = std::clamp(cosine, -1.0f, 1.0f);
        float radian = acosf(cosine);
        //----------------------------------------------------------------------------------
        // 回転追従
        //----------------------------------------------------------------------------------
        //右回転か左回転どちらが最短かを判定
        //cross_dirの軸を中心に回転させるだけで実現可能。
        float3 cross_dir = cross(display_dir_, dir_);
        //外積結果が使えない場合(同じ方向 or 逆方向)
        if(dot(cross_dir, cross_dir) < float1(0.00001f)) {
            cross_dir = float3(0.0f, 1.0f, 0.0f);
        }
        {
            matrix mat_rot_y = matrix::rotateAxis(cross_dir, radian * 0.1f);
            display_dir_     = mul(float4(display_dir_, 0.0f), mat_rot_y).xyz;
        }

        front = display_dir_;

        //frontの方向に併せてrightを追従させる(外積で方向を再計算)
        right = cross(up, front);
        right = normalize(right);
        //----------------------------------------------------------------------------------
        // ワールド行列を指定
        //----------------------------------------------------------------------------------
        world_mat[0] = float4(right, 0.0f);       //右方向ベクトル
        world_mat[1] = float4(up, 0.0f);          //上方向ベクトル
        world_mat[2] = float4(front, 0.0f);       //前方向ベクトル
        world_mat[3] = float4(position, 1.0f);    //位置座標
        //オーナーのワールド行列を更新
        owner->Matrix() = world_mat;
    }
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentAI::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"AI")) {
            ImGui::DragFloat(u8"タイムカウント", &lift_time_count_, 0.01f, 0.0f, 10.0f);
            // GUI上でオーナーから自分(SampleObjectController)を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

//---------------------------------------------------------------------------
//! @brief	投げたいときにtrueを返す
//---------------------------------------------------------------------------
bool ComponentAI::ThrowSignal()
{
    //持ち上げている時間が5秒以上ならtrueを返す
    if(lift_time_count_ >= 1.0f) {
        return true;
    }
    return false;
}

CEREAL_REGISTER_TYPE(ComponentAI)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentAI)
