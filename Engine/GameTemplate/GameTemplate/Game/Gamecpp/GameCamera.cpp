#include "stdafx.h"
#include "GameCamera.h"
#include"Player.h"
#define _USE_MATH_DEFINES //M_PI 円周率呼び出し
#include <math.h> 

GameCamera::GameCamera()
{
	//カメラを初期化。
	g_camera3D.SetPosition({ 0.0f, 100.0f, 300.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(10000.0f);
	c_State = toPlayer;
}


GameCamera::~GameCamera()
{
}

void GameCamera::Update()
{
	if (g_pad[0].IsPress(enButtonRB2) && c_State == toPlayer)
	{
		c_State == toEnemys;
	}
	else if (g_pad[0].IsPress(enButtonRB2) && c_State == toEnemys)
	{
		c_State == toPlayer;
	}

	switch (c_State)
	{
	case GameCamera::toPlayer:
		CameraRotate();
		break;
	case GameCamera::toEnemys:
		CameraLookEnemys();
		break;
	}
	//CVector3 targetPos = m_player->GetPosition();	//プレイヤーのポジションを取得。
	//targetPos.y += 100.0f;							//上から見たいので100.0f代入。
	//CVector3 position = targetPos;					//ポジションにプレイヤーのポジションを入れる。
	//position.z -= 400.0f;
	////パッドの入力量でカメラを回す。
	//float x = g_pad[0].GetRStickXF();	//X方向への移動処理。
	//float z = g_pad[0].GetRStickYF();	//Y方向への移動処理。
	//	//Y軸周りの回転
	////CQuaternion qRot;
	//m_rotate.SetRotationDeg(CVector3::AxisY(), 200.0f * x);
	//m_rotate.SetRotationDeg(CVector3::AxisY(), 200.0f * z);
	////後ろから見たので400.0fを代入
	//g_camera3D.SetTarget(targetPos);				//プレイヤーのポジションをターゲットに。
	//g_camera3D.SetPosition(position);				//カメラのポジションにpositionを
	//g_camera3D.SetRotation(m_rotate);
	//g_camera3D.Update();							//アップデート。
}

void GameCamera::CameraRotate()
{
	m_playerposition = m_player->GetPosition();
	CVector3 stickR;
	stickR.x = -g_pad[0].GetRStickXF();	//アナログスティックのxの入力量を取得。
	stickR.y = g_pad[0].GetRStickYF();	//アナログスティックのxの入力量を取得。
	stickR.z = 0.0f;

	m_playerposition.z -= 400.0f;
	//右スティックの入力
	//右スティック
	m_sdegreexz = -stickR.x * 5.0f;
	m_sdegreey = -stickR.y*5.0f;

	//回転度加算
	m_degreexz += m_sdegreexz;
	m_degreey += m_sdegreey;
	//上下方向のカメラ移動を制限
	if (m_degreey >= 70.0f) {
		m_degreey = 70.0f;
	}
	if (m_degreey <= -15.0f) {
		m_degreey = -15.0f;
	}
	//角度をラジアン単位に直す
	m_radianx = M_PI / 180 * m_degreexz;
	m_radiany = M_PI / 180 * m_degreey;
	Hutu();
	//視点
	g_camera3D.SetTarget(m_target);
	//座標
	g_camera3D.SetPosition(m_position);
	//カメラの更新。
	g_camera3D.Update();
}

void GameCamera::Hutu()
{
	m_target = { 0.0f,0.0f,0.0f };
	m_target.y += 140.0f;

	m_target += m_player->GetPosition();
	//注視点を計算する。
	//target.y += 200.0f;
	//Y軸周りに回転させる。
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisY(), m_radianx);
	m_toPos = { 0.0f, 0.0f, 1.0f };
	qRot.Multiply(m_toPos);
	//上下の回転。
	//まずは回す軸を計算する。
	CVector3 rotAxis;
	rotAxis.Cross(m_toPos, CVector3::AxisY());
	//ベクトルを正規化する。
	rotAxis.Normalize();
	qRot.SetRotation(rotAxis, m_radiany);
	qRot.Multiply(m_toPos);
	m_toPos *= m_r;
	m_position = m_target + m_toPos * 2.0f;

	m_toPos *= 4;
	m_target -= m_toPos;
	m_toPos *= 1.5f;
}

void GameCamera::CameraLookEnemys()
{
	for (auto enemy : m_goList) {
		if (enemy->GetIsDead() == false) {
			CVector3 diff = enemy->GetPosition();
			if (diff.Length() >= 400)
			{
				m_target = enemy->GetPosition();
			}
		}
	}
	g_camera3D.SetTarget(m_target);
	//座標
	g_camera3D.SetPosition(m_position);
	//カメラの更新。
	g_camera3D.Update();
}