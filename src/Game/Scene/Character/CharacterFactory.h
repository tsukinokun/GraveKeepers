//---------------------------------------------------------------------------
//!	@file	CharacterFactory.h
//! @brief	キャラクターを生成するファクトリークラス
//! @note   シングルトンパターンで実装
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
class Character;    //前方宣言

class CharacterFactory
{
private:
    //---------------------------------------------------------------------------
    // コンストラクタ
    //! @note シングルトンパターンのためprivateにする
    //---------------------------------------------------------------------------
    CharacterFactory();
    ~CharacterFactory();

    // コピーを禁止する
    CharacterFactory(const CharacterFactory&)            = delete;
    CharacterFactory& operator=(const CharacterFactory&) = delete;

public:
    //---------------------------------------------------------------------------
    // CharacterFactoryのインスタンスを取得する静的メソッド
    //! @return CharacterFactoryのインスタンス
    //---------------------------------------------------------------------------
    static CharacterFactory& Instance();

    //---------------------------------------------------------------------------
    // キャラクターを生成するメソッド
    //! @param[in] chara_name キャラクター名
    //! @return 生成したキャラクターのshared_ptr、存在しない場合はnullptr
    //---------------------------------------------------------------------------
    std::shared_ptr<Character> CreateCharacter(const std::string& chara_name);

    // 登録されているキャラクター名一覧を返す
    std::vector<std::string> GetRegisteredCharacterNames() const;

private:
    // 登録用マップ
    std::unordered_map<std::string, std::function<std::shared_ptr<Character>()>> chara_creators_;
};
