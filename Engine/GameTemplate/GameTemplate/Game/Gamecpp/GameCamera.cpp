#include "stdafx.h"
#include "GameCamera.h"
#include"Player.h"

GameCamera::GameCamera()
{
	//カメラを初期化。
	g_camera3D.SetPosition({ 0.0f, 100.0f, 300.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(10000.0f);
}


GameCamera::~GameCamera()
{
}

void GameCamera::Update()
{
	CVector3 targetPos = m_player->GetPosition();	//プレイヤーのポジションを取得。
	targetPos.y += 100.0f;							//上から見たいので100.0f代入。
	CVector3 position = targetPos;					//ポジションにプレイヤーのポジションを入れる。
	position.z -= 400.0f;							//後ろから見たので400.0fを代入
	g_camera3D.SetTarget(targetPos);				//プレイヤーのポジションをターゲットに。
	g_camera3D.SetPosition(position);				//カメラのポジションにpositionを
	g_camera3D.Update();							//アップデート。
}