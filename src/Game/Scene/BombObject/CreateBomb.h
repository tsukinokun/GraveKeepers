//---------------------------------------------------------------------------
//!	@file	CreateBomb.h
//! @brief	インゲームシーンの狼男
//! @author 田中南々子
//---------------------------------------------------------------------------

#include <System/Scene.h>
//前方宣言
class ComponentLiftable;
class ComponentCollisionSphere;
USING_PTR(CreateBomb);

class CreateBomb : public Object
{
public:
    BP_OBJECT_DECL(CreateBomb, u8"爆弾オブジェクトの生成")

    //@
    bool Init() override;    //!< 初期化

    void Update() override;    //!< 更新

private:
    const int CANDYBOMB_NUM_MAX_ = 5;
};
