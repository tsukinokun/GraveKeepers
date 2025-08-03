#pragma once
#include <System/Component/ComponentObjectController.h>
#include <System/Component/ComponentSpringArm.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentStatus.h>

void ComponentObjectController::Init()
{
    __super::Init();
}

void ComponentObjectController::Update()
{
    __super::Update();

    // オーナー(自分がAddComponentされたObject)を取得します
    // 処理されるときは必ずOwnerは存在しますので基本的にnullptrチェックは必要ありません
    auto owner = GetOwner();
    if(auto hp = owner->GetComponent<ComponentStatus>()) {
        //死亡で
        if(hp->IsDead()) {
            return;
        }
    }
    //----------------------------------------------------------------------------------
    // キャラクターの移動
    //----------------------------------------------------------------------------------
    auto   world_mat = owner->Matrix();
    float3 position  = world_mat[3].xyz;               //ワールド座標の一成分を取得
    float3 front     = normalize(world_mat[2].xyz);    //前方の単位ベクトルを取得
    float3 up        = normalize(world_mat[1].xyz);    //上方の単位ベクトルを取得
    float3 right     = normalize(world_mat[0].xyz);    //右の単位ベクトルを取得

    // 移動方向
    float3 move = float3(0.0f, 0.0f, 0.0f);
    if(IsKey(key_up_))
        move += {0, 0, -1};

    if(IsKey(key_down_))
        move += {0, 0, 1};

    if(IsKey(key_right_))
        move += {-1, 0, 0};

    if(IsKey(key_left_))
        move += {1, 0, 0};

    // 移動キーが押されているか?
    if(float1(0.0001f) < dot(move, move)) {
        // 斜めが押されていることを考慮し、
        // その方向の移動スピードを1とし、スピードを掛け合わせる
        move = normalize(move);
        dir_ = move;    // 内部の方向を更新
    }

    //移動
    position += move * move_speed_;

    float cosine = dot(display_dir_, dir_);    //ベクトルのなす角
    cosine       = std::clamp(cosine, -1.0f, 1.0f);
    float radian = acosf(cosine);
    //----------------------------------------------------------------------------------
    // 回転追従
    //----------------------------------------------------------------------------------
    //右回転か左回転どちらが最短かを判定
    //cross_dirの軸を中心に回転させるだけで実現可能。
    float3 cross_dir = cross(display_dir_, dir_);
    //外積結果が使えない場合(同じ方向 or 逆方向)
    if(dot(cross_dir, cross_dir) < float1(0.00001f)) {
        cross_dir = float3(0.0f, 1.0f, 0.0f);
    }
    {
        matrix mat_rot_y = matrix::rotateAxis(cross_dir, radian * 0.1f);
        display_dir_     = mul(float4(display_dir_, 0.0f), mat_rot_y).xyz;
    }

    front = display_dir_;

    //frontの方向に併せてrightを追従させる(外積で方向を再計算)
    right = cross(up, front);
    right = normalize(right);
    //----------------------------------------------------------------------------------
    // ワールド行列を指定
    //----------------------------------------------------------------------------------
    world_mat[0] = float4(right, 0.0f);       //右方向ベクトル
    world_mat[1] = float4(up, 0.0f);          //上方向ベクトル
    world_mat[2] = float4(-front, 0.0f);      //前方向ベクトル
    world_mat[3] = float4(position, 1.0f);    //位置座標
    //オーナーのワールド行列を更新
    owner->Matrix() = world_mat;

    //----------------------------------------------------------------------------------
    // カメラが存在している場合
    //----------------------------------------------------------------------------------
    if(auto camera = Scene::GetCurrentCamera().lock()) {
        // SpringArmのオブジェクトが自分の場合のみ
        auto cam_owner = camera->GetOwner();
        if(auto arm = cam_owner->GetComponent<ComponentSpringArm>()) {
            // そのターゲットはオーナーのオブジェクトであることが条件となる
            auto obj = arm->GetSpringArmObject().lock();
            if(obj.get() == owner) {
                // オブジェクトコントローラーにターゲットがいる場合
                if(auto target = target_.lock()) {
                    // 徐々に敵の方に向ける(1フレーム最大3度)
                    owner->SetRotationToPositionWithLimit(target->GetTranslate(), target_cam_side_speed_ * GetDeltaTime60());

                    // カメラローテーションをロック方向にしておくと
                    // 戻った時に違和感がない
                    cam_ry_ = owner->GetRotationAxisXYZ().y;
                    cam_rx_ = (cam_rx_ + target_cam_up_down) * 0.95f - target_cam_up_down;

                    arm->SetSpringArmRotate({cam_rx_, 0, 0});
                }
                else {
                    // ターゲットがない場合
                    // マウスで動作せる場合の処理
                    float mouse_ud = mouse_up_down_ * GetDeltaTime60();
                    float mouse_lr = mouse_left_right_ * GetDeltaTime60();
                    float cam_spd  = cam_speed_ * GetDeltaTime60();
                    if(use_mouse_) {
                        cam_rx_ += GetMouseMoveY() * (mouse_ud / 100.0f);
                        if(cam_rx_ > limit_cam_up_)
                            cam_rx_ = limit_cam_up_;
                        if(cam_rx_ < limit_cam_down_)
                            cam_rx_ = limit_cam_down_;
                        cam_ry_ += GetMouseMoveX() * (mouse_lr / 100.0f);
                        if(cam_ry_ > 360.0f)
                            cam_ry_ -= 360.0f;
                        if(cam_ry_ < -360.0f)
                            cam_ry_ += 360.0f;
                    }
                    // キーでカメラを移動させる
                    if(IsKey(cam_up_)) {
                        cam_rx_ += cam_spd;
                        if(cam_rx_ > limit_cam_up_)
                            cam_rx_ = limit_cam_up_;
                    }
                    if(IsKey(cam_down_)) {
                        cam_rx_ -= cam_spd;
                        if(cam_rx_ < limit_cam_down_)
                            cam_rx_ = limit_cam_down_;
                    }
                    if(IsKey(cam_right_)) {
                        cam_ry_ += cam_spd;
                        if(cam_ry_ > 360.0f)
                            cam_ry_ -= 360.0f;
                    }
                    if(IsKey(cam_left_)) {
                        cam_ry_ -= cam_spd;
                        if(cam_ry_ < -360.0f)
                            cam_ry_ += 360.0f;
                    }

                    // Y軸はオブジェクトそのものの向きを使う
                    owner->SetRotationAxisXYZ({0, cam_ry_, 0});
                    // 上下はSpringArmで制御しておく
                    arm->SetSpringArmRotate({cam_rx_, 0, 0});
                }
            }
        }
    }
}

ComponentObjectControllerPtr ComponentObjectController::SetMoveSpeed(const float speed)
{
    move_speed_ = speed;
    return std::dynamic_pointer_cast<ComponentObjectController>(shared_from_this());
}

ComponentObjectControllerPtr ComponentObjectController::SetRotateSpeed(const float speed)
{
    rot_speed_ = speed;
    return std::dynamic_pointer_cast<ComponentObjectController>(shared_from_this());
}

const float ComponentObjectController::GetMoveSpeed() const
{
    return move_speed_;
}

const float ComponentObjectController::GetRotateSpeed() const
{
    return rot_speed_;
}

ComponentObjectControllerPtr ComponentObjectController::SetKeys(int up, int down, int left, int right)
{
    // 移動キーの設定
    key_up_    = up;
    key_down_  = down;
    key_left_  = left;
    key_right_ = right;
    return std::dynamic_pointer_cast<ComponentObjectController>(shared_from_this());
}

ComponentObjectControllerPtr ComponentObjectController::SetCameraKeys(int up, int down, int left, int right)
{
    // カメラキーの設定
    cam_up_    = up;
    cam_down_  = down;
    cam_left_  = left;
    cam_right_ = right;
    return std::dynamic_pointer_cast<ComponentObjectController>(shared_from_this());
}

ComponentObjectControllerPtr ComponentObjectController::SetTarget(ObjectPtr target)
{
    target_ = target;
    return std::dynamic_pointer_cast<ComponentObjectController>(shared_from_this());
}

ComponentObjectControllerPtr ComponentObjectController::SetCameraRotate(float3 rot)
{
    cam_rx_ = rot.x;
    cam_ry_ = rot.y;
    return std::dynamic_pointer_cast<ComponentObjectController>(shared_from_this());
}

ComponentObjectControllerPtr ComponentObjectController::SetUseMouse(bool use)
{
    use_mouse_ = use;
    return std::dynamic_pointer_cast<ComponentObjectController>(shared_from_this());
}

bool ComponentObjectController::IsUseMouse() const
{
    return use_mouse_;
}

void ComponentObjectController::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"ObjectController")) {
            // GUI上でオーナーから自分(SampleObjectController)を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            // 移動の基本情報
            ImGui::DragFloat(u8"移動速度", &move_speed_, 0.1f);
            ImGui::DragFloat(u8"移動回転角度", &rot_speed_, 1.0f);

            // カメラの基本情報
            ImGui::Checkbox(u8"カメラ方向にMouseを使用", &use_mouse_);
            ImGui::DragFloat(u8"カメラ左右回転スピード", &mouse_left_right_);
            ImGui::DragFloat(u8"カメラ上下回転スピード", &mouse_up_down_);

            ImGui::DragFloat(u8"見る方向上リミット", &limit_cam_up_);
            ImGui::DragFloat(u8"見る方向下リミット", &limit_cam_down_);

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

CEREAL_REGISTER_TYPE(ComponentObjectController)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentObjectController)
