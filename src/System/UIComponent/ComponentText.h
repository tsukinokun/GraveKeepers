#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentText);

class ComponentText : public Component
{
public:
    BP_COMPONENT_DECL(ComponentText, u8"文字列コンポーネント");
    //! @{
    //	初期化処理
    void Init() override;

    //描画処理
    void LateDraw() override;

    //ImGui
    void GUI() override;

    //文字列の設定
    //! @param str [in] 描画する文字列の設定
    std::shared_ptr<ComponentText> SetText(const std::string_view& str);

    //文字色の設定
    //! @param		text_color [in] 描画する文字色の設定
    //! @param		edge_color [in] 描画する文字のふち色の設定
    //! @details	第二引数はデフォルト引数で黒に設定されています。
    std::shared_ptr<ComponentText> SetColor(int text_color, int edge_color = 0);
    //! @}

private:
    int         text_color_ = 0;    //文字の色
    int         edge_color_ = 0;    //文字の縁の色
    std::string str_;               //描画する文字列

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentText, 3);
