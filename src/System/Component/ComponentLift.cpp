//---------------------------------------------------------------------------
//!	@file	ComponentLift.cpp
//! @brief	持ち上げ機能コンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Component/ComponentLift.h>
#include <System/Component/ComponentLiftable.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentRigidbody.h>

//---------------------------------------------------------------------------
//! @brief	初期化
//---------------------------------------------------------------------------
void ComponentLift::Init()
{
    __super::Init();
}

//---------------------------------------------------------------------------
//! @brief	更新処理
//---------------------------------------------------------------------------
void ComponentLift::Update()
{
    __super::Update();
    auto owner = GetOwner();

    //持ち上げる処理
    if(auto lift_obj = lift_object_.lock()) {
        lift_obj->GetComponent<ComponentLiftable>()->SetLiftedFlag(true);
        auto   owner_col  = owner->GetComponent<ComponentCollisionCapsule>();    //オーナーのコリジョンを取得
        float3 end        = owner->GetTranslate();                               //高さ
        end.y            += (owner_col->GetHeight() + 2.0f);                     //終点座標は頭なので、高さの半分を足す。
        //持ち上げ対象を持ち上げる
        lift_obj->SetTranslate(end);

        //投げる
        if(conditions_for_throw_()) {
            if(auto lift_obj = lift_object_.lock()) {
                auto   lift_rb        = lift_obj->GetComponent<ComponentRigidbody>();
                float3 throw_impulse_ = float3(0.0f, throw_virtical_power_, 0.0f);
                float3 owner_rot      = owner->GetRotationAxisXYZ();    //オーナーの向きを取得
                //オーナーのy軸回転から、throw_impulse_のxとzを設定
                throw_impulse_.x = -throw_horizontal_power_ * sinf(D2R(owner_rot.y));
                throw_impulse_.z = -throw_horizontal_power_ * cosf(D2R(owner_rot.y));
                lift_rb->AddImpulse(throw_impulse_);
                auto lift_col = lift_obj->GetComponent<ComponentCollision>();
                lift_col->SetEnableFlag(true);
                lift_col->UseGravity();
            }
            lift_object_.reset();
            return;
        }
    }

    //持ち上げ第一条件が満たされたら
    if(conditions_for_lifting_()) {
        //一番近いオブジェクトを取得する
        if(lift_object_.lock() == nullptr)    //監視対象が存在しなければループを回す
        {
            float1 most_near_distance = std::numeric_limits<float>::max();    //とりあえず大きい数で初期化
            for(auto obj : Scene::Object::GetArray<Object>()) {
                //オブジェクトとオーナーの名前が同じなら戻す
                if(owner->GetName().data() == obj->GetName().data()) {
                    continue;
                }
                //デフォルト名称を取得
                auto def_name = obj->GetNameDefault();
                //デフォルト名が無視するオブジェクトなら戻す
                //オブジェクトの名前が無視を行う名前なら、コンティニューを行う
                if(!CheckLiftObjName(def_name.data())) {
                    continue;
                }
                //オブジェクトが持ち上げられ中ならコンティニュー
                if(obj->GetComponent<ComponentLiftable>()->IsLifted()) {
                    continue;
                }
                //オーナーが持ち上げられ中なら持ち上げない
                if(owner->GetComponent<ComponentLiftable>()->IsLifted()) {
                    continue;
                }
                //オブジェクトが持ち上げ中ならコンテニュー
                if(auto obj_lif_comp = obj->GetComponent<ComponentLift>()) {
                    if(obj_lif_comp->IsLifting()) {
                        continue;
                    }
                }
                //オーナーの正面ベクトルを取得
                float3 owner_front = float3(0.0f, 0.0f, 0.0f);
                float3 owner_rot   = owner->GetRotationAxisXYZ();
                owner_front.x      = -1.0f * sinf(D2R(owner_rot.y));
                owner_front.z      = -1.0f * cosf(D2R(owner_rot.y));
                //一応正規化
                owner_front = normalize(owner_front);
                //オブジェクトとオーナーのベクトルを取得
                float3 vec_owner_to_obj = obj->GetMatrix().translate() - owner->GetMatrix().translate();
                //単位ベクトルを求める
                float3 normalize_vec = normalize(vec_owner_to_obj);
                //オブジェクトと持ち上げオーナーの内積を求める
                float obj_to_owner_dot = dot(owner_front, normalize_vec);
                //内積から角度を求める
                float rad = acosf(obj_to_owner_dot);
                //角度が持ち上げ可能角度におさまっていなければ
                if(rad > D2R(lift_angle_)) {
                    continue;    //コンティニュー
                }
                //ベクトルの長さが持ち上げられる範囲を超えていたら
                if(length(vec_owner_to_obj) > float1(lift_distance_)) {
                    continue;    //コンティニュー
                }
                //ベクトルの長さがこれまでに一番近かったオブジェクトよりも近いなら、監視対象オブジェクトを代入して、長さも代入する
                if(length(vec_owner_to_obj) < most_near_distance) {
                    most_near_distance = length(vec_owner_to_obj);
                    lift_object_       = obj;    //持ち上げオブジェクトを代入
                }
            }
            if(auto obj = lift_object_.lock()) {
                obj->GetComponent<ComponentLiftable>()->SetLiftedFlag(true);
                auto lift_col = obj->GetComponent<ComponentCollision>();
                lift_col->SetEnableFlag(false);
                lift_col->UseGravity(false);
            }
            return;
        }
    }
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------

void ComponentLift::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"Lift")) {
            // GUI上でオーナーから自分(SampleObjectController)を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

//---------------------------------------------------------------------------
//! @brief	持ち上げ条件を記述
//---------------------------------------------------------------------------

void ComponentLift::SetConditionsForLifting(std::function<bool()> conditions)
{
    conditions_for_lifting_ = conditions;
}

//---------------------------------------------------------------------------
//! @brief	投げる条件記述
//---------------------------------------------------------------------------

void ComponentLift::SetConditionsForThrow(std::function<bool()> conditions)
{
    conditions_for_throw_ = conditions;
}

//---------------------------------------------------------------------------
//! @brief	名前から、監視対象になるオブジェクトかどうかを返す
//---------------------------------------------------------------------------
bool ComponentLift::CheckLiftObjName(const std::string& name)
{
    for(int i = 0; i < IGNORE_NAMES_.size(); i++) {
        if(name == IGNORE_NAMES_[i]) {
            return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------
//! @brief	持ち上げ中か否かを返す関数
//---------------------------------------------------------------------------
bool ComponentLift::IsLifting()
{
    if(lift_object_.lock() != nullptr) {
        return true;
    }
    return false;
}

CEREAL_REGISTER_TYPE(ComponentLift)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentLift)
