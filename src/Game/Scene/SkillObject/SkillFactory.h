//---------------------------------------------------------------------------
//!	@file	SkillFactory.h
//! @brief	スキルを生成するファクトリークラス
//! @note   シングルトンパターンで実装
//! @author 田中南々子
//---------------------------------------------------------------------------
#pragma once
class SkillObjectBase;    //前方宣言

class SkillFactory
{
private:
    //---------------------------------------------------------------------------
    // コンストラクタ
    //! @note シングルトンパターンのためprivateにする
    //---------------------------------------------------------------------------
    SkillFactory();

    // コピーを禁止する
    SkillFactory(const SkillFactory&)            = delete;
    SkillFactory& operator=(const SkillFactory&) = delete;

public:
    //---------------------------------------------------------------------------
    // SkillFactoryのインスタンスを取得する静的メソッド
    //! @return SkillFactoryのインスタンス
    //---------------------------------------------------------------------------
    static SkillFactory& Instance();

    //---------------------------------------------------------------------------
    // スキルを生成するメソッド
    //! @param[in] skill_name スキル名
    //! @return 生成したスキルのshared_ptr、存在しない場合はnullptr
    //---------------------------------------------------------------------------
    std::shared_ptr<SkillObjectBase> CreateSkill(const std::string& skill_name);

    // 登録されているキャラクター名一覧を返す
    std::vector<std::string> GetRegisteredSkillNames() const;

private:
    // 登録用マップ
    std::unordered_map<std::string, std::function<std::shared_ptr<SkillObjectBase>()>> skill_creators_;
};
