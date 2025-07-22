#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentString);

//! 剛体コンポーネント
class ComponentString : public Component
{
public:
    BP_COMPONENT_DECL(ComponentString, u8"文字列コンポーネント");
    //! @{
    //	初期化処理
    void Init() override;

    //	物理演算
    void LateDraw() override;

    //ImGui
    void GUI() override;

    //文字列の設定
    //! @param str [in] 描画する文字列の設定
    std::shared_ptr<ComponentString> SetString(const std::string_view& str);
    //! @}

private:
    std::string_view str_;    //描画する文字列

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentString, 3);
