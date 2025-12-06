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
#include <System/Component/ComponentStatus.h>
#include <Game/Scene/Character/Base/Character.h>

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
    is_just_lifted_ = false;    //持ち上げたフレームか否かのフラグを初期化
    auto owner      = GetOwnerPtr();
    if(auto hp = owner->GetComponent<ComponentStatus>()) {
        //死亡で
        if(hp->IsDead()) {
            return;
        }
    }
    //持ち上げる処理
    if(auto lift_obj = lift_object_.lock()) {
        lift_obj->GetComponent<ComponentLiftable>()->SetLiftedFlag(true);
        auto   owner_col  = owner->GetComponent<ComponentCollisionCapsule>();    //オーナーのコリジョンを取得
        float3 end        = owner->GetTranslate();                               //高さ
        end.y            += (owner_col->GetHeight() + 4.0f);                     //終点座標は頭なので、高さの半分を足す。
        //持ち上げ対象を持ち上げる
        lift_obj->SetTranslate(end);

        //投げる
        if(conditions_for_throw_()) {
            if(auto lift_obj = lift_object_.lock()) {
                auto   lift_rb        = lift_obj->GetComponent<ComponentRigidbody>();
                float3 throw_impulse  = float3(0.0f, throw_virtical_power_, 0.0f);
                float3 owner_rot      = owner->GetRotationAxisXYZ();    //オーナーの向きを取得
                owner_rot.y          += 180.0f;                         //座標系の関係でyを180度回転する、オブジェクトの背中が正面
                //オーナーのy軸回転から、throw_impulse_のxとzを設定
                throw_impulse.x = -throw_horizontal_power_ * sinf(D2R(owner_rot.y));
                throw_impulse.z = -throw_horizontal_power_ * cosf(D2R(owner_rot.y));
                lift_rb->AddImpulse(throw_impulse);
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
                //持ち上げられ機能コンポーネントを取得
                if(auto lift1able_comp = obj->GetComponent<ComponentLiftable>()) {
                    //オブジェクトが持ち上げられ中ならコンティニュー
                    if(obj->GetComponent<ComponentLiftable>()->CanBeLifted()) {
                        continue;
                    }
                }
                else {
                    //ComponentLiftableがついていなかったらコンティニュー
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
                float3 owner_front  = float3(0.0f, 0.0f, 0.0f);
                float3 owner_rot    = owner->GetRotationAxisXYZ();
                owner_rot.y        += 180.0f;    //座標系の関係でyを180度回転する、オブジェクトの背中が正面
                owner_front.x       = -1.0f * sinf(D2R(owner_rot.y));
                owner_front.z       = -1.0f * cosf(D2R(owner_rot.y));
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

                // 真上・真下方向ベクトル
                float3 up       = float3(0.0f, 1.0f, 0.0f);
                float3 down     = float3(0.0f, -1.0f, 0.0f);
                float  up_dot   = dot(up, normalize_vec);
                float  down_dot = dot(down, normalize_vec);

                // 真上・真下に近い場合は特別に許容
                if(up_dot > 0.8f || down_dot > 0.8f) {
                    //角度が持ち上げ可能角度におさまっていなければ
                    if(rad > D2R(lift_angle_)) {
                        continue;    //コンティニュー
                    }
                }

                //ベクトルの長さが持ち上げられる範囲を超えていたら
                if(length(vec_owner_to_obj) > float1(lift_distance_)) {
                    continue;    //コンティニュー
                }
                //ベクトルの長さがこれまでに一番近かったオブジェクトよりも近いなら、監視対象オブジェクトを代入して、長さも代入する
                if(length(vec_owner_to_obj) < most_near_distance) {
                    most_near_distance = length(vec_owner_to_obj);
                    is_just_lifted_    = true;    //持ち上げたフレームフラグを立てる
                    lift_object_       = obj;     //持ち上げオブジェクトを代入
                }
            }
            if(auto obj = lift_object_.lock()) {
                if(auto liftable_comp = obj->GetComponent<ComponentLiftable>()) {
                    liftable_comp->SetLiftedFlag(true);                                         //持ち上げ中にする
                    liftable_comp->SetLiftCharacter(dynamic_pointer_cast<Character>(owner));    //持ち上げているキャラクターをセット
                    liftable_comp->SetCannotBeLifted();                                         //持ち上げ不可にしておく
                }
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

//--------------------------------------------------------------------
//! @brief 持ち上げたフレームならtrueを返す関数
//--------------------------------------------------------------------
bool ComponentLift::IsJustLifted()
{
    return is_just_lifted_;
}

CEREAL_REGISTER_TYPE(ComponentLift)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentLift)
