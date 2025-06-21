#pragma once
#include <System/Component/ComponentLift.h>

void ComponentLift::Init()
{
    __super::Init();
}

void ComponentLift::Update()
{
    __super::Update();
    auto owner = GetOwner();
    //持ち上げ第一条件が満たされたら
    if(conditions_for_lifting_()) {
        //一番近いオブジェクトを取得する
        if(lift_object_.lock() == nullptr)    //監視対象が存在しなければループを回す
        {
            float1 most_near_distance = std::numeric_limits<float>::max();    //とりあえず大きい数で初期化
            for(auto obj : Scene::Object::GetArray<Object>()) {
                //オブジェクトとオーナーの名前が同じなら戻す
                if(owner->GetName() == obj->GetName()) {
                    continue;
                }
                //デフォルト名称を取得
                auto def_name = obj->GetNameDefault();
                //デフォルト名が無視するオブジェクトなら戻す

                //オブジェクトとオーナーのベクトルを取得
                float3 vec_owner_to_obj = owner->GetMatrix().translate() - obj->GetMatrix().translate();
                //ベクトルの長さがこれまでに一番近かったオブジェクトよりも近いなら、監視対象オブジェクトを代入して、長さも代入する
                if(length(vec_owner_to_obj) < most_near_distance) {
                    most_near_distance = length(vec_owner_to_obj);
                }
                lift_object_ = obj;    //持ち上げオブジェクトを代入
            }
        }
    }
}

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

//ラムダ式で持ち上げ条件を記述
void ComponentLift::SetConditionsForLifting(std::function<bool()> conditions)
{
    conditions_for_lifting_ = conditions;
}

void ComponentLift::SetConditionsForThrow(std::function<bool()> conditions)
{
    conditions_for_throw_ = conditions;
}

CEREAL_REGISTER_TYPE(ComponentLift)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentLift)
