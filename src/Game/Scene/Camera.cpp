//---------------------------------------------------------------------------
//!	@file	Camera.cpp
//! @brief	ゲームカメラ
//---------------------------------------------------------------------------
#include "Camera.h"
#include <System/Component/ComponentCollisionModel.h>
#include <System/Component/ComponentSpringArm.h>
#include <Game/Scene/Character/Base/Character.h>
#include <Game/System/HlslppUseful.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Camera::Init()
{
    __super::Init();
    //---------------------------------------------------------------------------------
    // カメラコンポーネントの追加と初期設定
    //---------------------------------------------------------------------------------
    auto cam_comp = AddComponent<ComponentCamera>();
    cam_comp->SetPositionAndTarget({0, 50, 100}, {0, 0, 0});
    //オブジェクト名をセット
    SetName(u8"Camera");
    //---------------------------------------------------------------------------------
    // 更新処理の入れ込み
    //---------------------------------------------------------------------------------
    {
        // 更新処理(ラムダ式)
        auto update_proc = [cam_comp]() {
            // キャラクター全員の位置を取得してベクターに格納
            std::vector<float3> character_positions;
            for(const auto& actor : Scene::Object::GetArray<Character>()) {
                //キャラクターが死亡状態なら
                if(!actor->IsAlive()) {
                    //位置取得を格納する処理を行わない
                    continue;
                }
                character_positions.push_back(actor->GetTranslate());
            }
            // キャラクターの中心位置を取得
            float3 center_position = CalculateCenter(character_positions);
            //カメラのターゲットを取得
            float3 prev_target = cam_comp->GetTarget();
            // ターゲットをキャラクターの中心に少しずつ近づける
            float3 curr_target = lerp(prev_target, center_position, 0.1f);
            // カメラの視点をキャラクターの中心に設定
            cam_comp->SetPositionAndTarget({0, 50, 100}, curr_target);
        };
        // カメラの更新処理処理登録
        SetProc("update_proc", update_proc, ProcTiming::Update, ProcPriority::NONE);
    }
    return true;
}
