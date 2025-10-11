//---------------------------------------------------------------------------
//!	@file	FontKey.h
//! @brief	フォントのキー構造体
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
struct FontKey
{
    std::string fontName;
    int         size;
    int         thickness;
    int         type;

    // コンストラクタ
    FontKey(const std::string& name, int s, int t = 1, int ty = DX_FONTTYPE_NORMAL)
        : fontName(name)
        , size(s)
        , thickness(t)
        , type(ty)
    {
    }

    // 等価比較演算子
    //! @param other [in] 自身と比較したいフォントキー
    //! @retval 等価か否か
    bool operator==(const FontKey& other) const
    {
        return fontName == other.fontName && size == other.size && thickness == other.thickness && type == other.type;
    }
};

// ハッシュ関数の定義（unordered_map用）
namespace std {
template <>
struct hash<FontKey>
{
    std::size_t operator()(const FontKey& k) const
    {
        return hash<std::string>()(k.fontName) ^ (hash<int>()(k.size) << 1) ^ (hash<int>()(k.thickness) << 2) ^ (hash<int>()(k.type) << 3);
    }
};
}    // namespace std
