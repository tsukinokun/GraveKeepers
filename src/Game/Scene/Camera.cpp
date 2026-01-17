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

    // カメラコンポーネント作成
    auto cam_comp = AddComponent<ComponentCamera>();
    cam_comp->SetPositionAndTarget({0, 50, 100}, {0, 0, 0});

    SetName(u8"Camera");

    // 更新処理
    auto update_proc = [cam_comp]() {
        //------------------------------------------------------------
        // 生存キャラクターの位置を収集
        //------------------------------------------------------------
        std::vector<float3> positions;
        positions.reserve(16);

        for(const auto& actor : Scene::Object::GetArray<Character>()) {
            if(actor->IsAlive()) {
                positions.push_back(actor->GetTranslate());
            }
        }

        if(positions.empty()) {
            return;    // 生存キャラがいないなら何もしない
        }

        //------------------------------------------------------------
        // 中心位置を計算
        //------------------------------------------------------------
        float3 center = CalculateCenter(positions);

        //------------------------------------------------------------
        // キャラの散らばり具合からズーム距離を決定
        //------------------------------------------------------------
        float max_dist = 0.0f;
        for(const auto& p : positions) {
            max_dist = std::max(max_dist, static_cast<float>(length(p - center)));
        }

        // 距離を 0〜50 の範囲で正規化
        float t = std::clamp(max_dist / 50.0f, 0.0f, 1.0f);

        // カメラ距離を補間（近い→40 / 遠い→150）
        float target_distance = lerp(static_cast<float1>(40.0f), static_cast<float1>(150.0f), t);

        //------------------------------------------------------------
        // カメラ位置とターゲットをスムーズに補間
        //------------------------------------------------------------
        float3 prev_target = cam_comp->GetTarget();
        float3 curr_target = lerp(prev_target, center, 0.1f);

        float3 prev_pos    = cam_comp->GetPosition();
        float3 desired_pos = float3(0, 50, target_distance);
        float3 curr_pos    = lerp(prev_pos, desired_pos, 0.1f);

        //------------------------------------------------------------
        // カメラ更新
        //------------------------------------------------------------
        cam_comp->SetPositionAndTarget(curr_pos, curr_target);
    };

    SetProc("update_proc", update_proc, ProcTiming::Update, ProcPriority::NONE);

    return true;
}
