//---------------------------------------------------------------------------
//!	@file	ComponentGauge.cpp
//! @brief	画像コンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentGauge.h"
#include "ComponentTransformUI.h"
#include <Game/system/ImageBuffer.h>

//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentGauge::Init()
{    // 初期化処理
    __super::Init();
    //---------------------------------------------------------------------------
    //  UI描画を登録
    //---------------------------------------------------------------------------
    auto draw_ui = [this]() {
        auto   owner      = GetOwner();         //オーナーを取得
        float3 adjustment = GetAdjustment();    // 調整値
        float3 pos        = float3(0.0f, 0.0f, 0.0f);
        pos               = owner->GetTranslate() + adjustment;
        float3 scale      = owner->GetScaleAxisXYZ();
        //サイズは、Transformの平均
        float size = (scale.x + scale.y + scale.z) / 3.0f;    // 平均値をとる
    };
    SetProc("UIDraw", draw_ui, ProcTiming::UI, static_cast<ProcPriority>(NONE));
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentGauge::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"ゲージコンポーネント")) {
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

//---------------------------------------------------------------------------
//! @brief ゲージの割合を設定する関数
//---------------------------------------------------------------------------
std::shared_ptr<ComponentGauge> ComponentGauge::SetGaugeSize(const int2& size)
{
    gauge_size_ = size;    // ゲージのサイズを設定
    return dynamic_pointer_cast<ComponentGauge>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief ゲージの割合を設定する関数
//---------------------------------------------------------------------------
std::shared_ptr<ComponentGauge> ComponentGauge::SetGaugeRate(float rate)
{
    gauge_rate_ = rate;    // ゲージの割合を設定
    return dynamic_pointer_cast<ComponentGauge>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief ゲージの色を設定する関数
//---------------------------------------------------------------------------
std::shared_ptr<ComponentGauge> ComponentGauge::SetGaugeColor(float color)
{
    color_ = color;    // ゲージの色を設定
    return dynamic_pointer_cast<ComponentGauge>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief  ゲージ座標の補正値を取得する関数
//---------------------------------------------------------------------------
float3 ComponentGauge::GetAdjustment() const
{
    auto   owner      = GetOwner();                  //オーナーを取得
    float3 adjustment = float3(0.0f, 0.0f, 0.0f);    // 調整値(Alignmentに合わせて)
    if(auto comp_transform = owner->GetComponent<ComponentTransformUI>()) {
        ComponentTransformUI::Alignment alignment = comp_transform->GetAlignment();
        //配置位置(縦)
        switch(static_cast<int>(alignment) / 3) {
        case 0:
            adjustment.y = (-gauge_size_.y * 0.5f);
            break;    // 上寄せ
        case 1:
            adjustment.y = 0.0f;
            break;    // 中央寄せ
        case 2:
            adjustment.y = (gauge_size_.y * 0.5f);
            break;    // 下寄せ
        }
        //配置位置(横)
        switch(static_cast<int>(alignment) % 3) {
        case 0:
            adjustment.x = (-gauge_size_.x * 0.5f);
            break;    // 左寄せ
        case 1:
            adjustment.x = 0.0f;
            break;    // 中央寄せ
        case 2:
            adjustment.x = (gauge_size_.x * 0.5f);
            break;    // 右寄せ
        }
    }
    return adjustment;
}

CEREAL_REGISTER_TYPE(ComponentGauge)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentGauge)
