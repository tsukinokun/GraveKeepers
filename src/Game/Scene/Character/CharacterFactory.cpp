//---------------------------------------------------------------------------
//!	@file	CharacterFactory.h
//! @brief	キャラクターを生成するファクトリークラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "CharacterFactory.h"
#include <Game/Scene/Character/Zombie/Zombie.h>
#include <Game/Scene/Character/Witch/Witch.h>
#include <Game/Scene/Character/Pumpking/Pumpking.h>
#include <Game/Scene/Character/Werewolf/Werewolf.h>

//---------------------------------------------------------------------------
//! @brief コンストラクタ
//---------------------------------------------------------------------------
CharacterFactory::CharacterFactory()
{
    //---------------------------------------------------------------------------
    // 登録用マップの初期化
    //---------------------------------------------------------------------------
    chara_creators_["Zombie"]   = []() { return Scene::Object::Create<Zombie>(); };      // ゾンビちゃんの生成処理
    chara_creators_["Werewolf"] = []() { return Scene::Object::Create<Werewolf>(); };    // 狼男くんの生成処理
    chara_creators_["Pumpking"] = []() { return Scene::Object::Create<Pumpking>(); };    // パンプキングの生成処理
    chara_creators_["Witch"]    = []() { return Scene::Object::Create<Witch>(); };       // ウィッチちゃんの生成処理
}

CharacterFactory::~CharacterFactory()
{
    // デストラクタ
    //解放
    chara_creators_.clear();
}

//---------------------------------------------------------------------------
//! @brief CharacterFactoryのインスタンスを取得する静的メソッド
//---------------------------------------------------------------------------
CharacterFactory& CharacterFactory::Instance()
{
    static CharacterFactory instance;    // ここで一度だけ生成
    return instance;
}

//---------------------------------------------------------------------------
//! @brief キャラクターを生成するメソッド
//---------------------------------------------------------------------------
std::shared_ptr<Character> CharacterFactory::CreateCharacter(const std::string& chara_name)
{
    auto it = chara_creators_.find(chara_name);
    if(it != chara_creators_.end()) {
        // 登録されている場合、対応する生成関数を呼び出してキャラクターを生成
        return it->second();
    }
    // 登録されていない場合はnullptrを返す
    return nullptr;
}

std::vector<std::string> CharacterFactory::GetRegisteredCharacterNames() const
{
    std::vector<std::string> names;
    names.reserve(chara_creators_.size());
    for(const auto& kv : chara_creators_) names.push_back(kv.first);
    return names;
}
