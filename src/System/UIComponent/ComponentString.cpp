//---------------------------------------------------------------------------
//!	@file	ComponentString.cpp
//! @brief	文字列コンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentString.h"

//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentString::Init()
{    // 初期化処理
    __super::Init();
}

//---------------------------------------------------------------------------
//! @brief	UI描画
//---------------------------------------------------------------------------
void ComponentString::LateDraw()
{
    __super::LateDraw();
    auto   owner = GetOwner();
    float3 pos   = float3(0.0f, 0.0f, 0.0f);
    pos          = owner->GetTranslate();
    DrawString(pos.x, pos.y, str_.data(), text_color_, edge_color_);
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentString::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"文字列コンポーネント")) {
            // GUI上でオーナーから自分(SampleObjectController)を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

//---------------------------------------------------------------------------
//! @brief	文字列の設定
//---------------------------------------------------------------------------
std::shared_ptr<ComponentString> ComponentString::SetString(const std::string_view& str)
{
    str_ = std::string(str);    // コピーして保持
    return dynamic_pointer_cast<ComponentString>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief	文字列色の設定
//---------------------------------------------------------------------------
std::shared_ptr<ComponentString> ComponentString::SetColor(int text_color, int edge_color)
{
    text_color_ = text_color;    // 文字色を設定
    edge_color_ = edge_color;    // 縁取り色を設定
    return dynamic_pointer_cast<ComponentString>(shared_from_this());
}
CEREAL_REGISTER_TYPE(ComponentString)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentString)
