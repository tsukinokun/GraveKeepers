//---------------------------------------------------------------------------
//! @file   SceneSample.cpp
//! @brief  サンプルシーン
//---------------------------------------------------------------------------
#include "SceneSample.h"

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool SceneSample::Init()
{
    // 仮モデルの読み込み
    model_ = MV1LoadModel("data/Sample/Player/model.mv1");

    return true;
}

//---------------------------------------------------------------------------
//! 更新
//! @param  [in]    delta   経過時間
//---------------------------------------------------------------------------
void SceneSample::Update()
{
    float3 right = float3(1.0f, 0.0f, 0.0f);    // 右方向
    float3 front = float3(0.0f, 0.0f, 1.0f);    // 前方向

    // 垂直なベクトル計算テスト
    // front = cross(front, right);
    // front = normalize(front);

    float speed = 0.1f;

    // 速度変更
    if(IsKey(KEY_INPUT_LSHIFT) || IsKey(KEY_INPUT_RSHIFT)) {
        speed *= 5.0f;
    }

    float3 move = 0.0f;    // 移動方向

    // 左右
    if(IsKey(KEY_INPUT_RIGHT)) {
        move += right;
    }
    if(IsKey(KEY_INPUT_LEFT)) {
        move -= right;
    }

    // 前後
    if(IsKey(KEY_INPUT_UP)) {
        move += front;
    }
    if(IsKey(KEY_INPUT_DOWN)) {
        move -= front;
    }

    // 速度を均一化 (速度ベクトルの正規化)
    // length()を使用すると内部の平方根√で0除算が発生してしまうため注意
    // "長さ" も "長さの2乗" も 0 になるかどうかの判定をする上では同じように利用できる
    if(0.0001f < dot(move, move).x) {    //if( 0.0001f < length(move).x ) {
        move = normalize(move);
    }

    // 移動
    position_ += move * speed;

    // 仮モデルの設定
    MV1SetPosition(model_, cast(position_));
    MV1SetRotationXYZ(model_, {D2R(0.0f), 0.0f, 0.0f});
    MV1SetScale(model_, {0.01f, 0.01f, 0.01f});

    //----------------------------------------------------------
    // カメラの設定
    //----------------------------------------------------------

    //                                  位置 Position  ,    注視点 LookAt
    SetCameraPositionAndTarget_UpVecY({0.0f, 10.0f, -20.0f}, {0.0f, 0.0f, 0.0f});

    // 画角(Field of view) を設定
    SetupCamera_Perspective(60.0f * DegToRad);
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void SceneSample::Draw()
{
    // 仮モデルの描画
    MV1DrawModel(model_);
}

//---------------------------------------------------------------------------
//! 終了
//---------------------------------------------------------------------------
void SceneSample::Exit()
{
    MV1DeleteModel(model_);
}

//---------------------------------------------------------------------------
//! GUI表示
//---------------------------------------------------------------------------
void SceneSample::GUI()
{
}
