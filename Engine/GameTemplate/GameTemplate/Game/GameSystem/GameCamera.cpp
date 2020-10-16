#include "stdafx.h"
#include "GameCamera.h"
#include"Chara/Player.h"
#define _USE_MATH_DEFINES //M_PI 円周率呼び出し
#include <math.h> 

GameCamera::GameCamera()
{
	//カメラを初期化。
	g_camera3D.SetPosition({ 0.0f, 100.0f, 300.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(10000.0f);
	c_state = toPlayer;
}

GameCamera::~GameCamera()
{
}

bool GameCamera::Start()
{
	m_player->AddEventListener(this);
	return true;
}

void GameCamera::Update()
{
	State();
}

void GameCamera::CameraRotate()
{
	m_playerPosition = m_player->GetPosition();
	CVector3 stickR;
	stickR.x = -g_pad[0].GetRStickXF();	//アナログスティックのxの入力量を取得。
	stickR.y = g_pad[0].GetRStickYF();	//アナログスティックのxの入力量を取得。
	stickR.z = 0.0f;

	m_playerPosition.z -= 400.0f;
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
	if (m_degreey <= 5.0f) {
		m_degreey = 5.0f;
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
	//if (damage_flag) {
		//return;
	//}
	if (!damage_flag) {
		m_target = m_player->GetPosition();
		m_target.y += 140.0f;
	}
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
	if (!damage_flag) {
		m_position = m_target + m_toPos;
	}
	m_toPos *= 1.5f;
}

void GameCamera::CameraLookEnemys()
{
	//プレイヤーの座標、エネミーの座標取得。
	m_playerPosition = m_player->GetPosition();
	m_enemyPosition = m_player->GetRookEnemyPos();
	//注視点はエネミーの座標にします。
	m_target = m_enemyPosition;
	//エネミーからプレイヤーに伸びるベクトルを求めます。
	CVector3 pos = m_playerPosition - m_enemyPosition;
	//カメラの高さは一定にしたいので、y成分を0にします。
	pos.y = 0.0f;
	//ベクトルを正規化します。
	pos.Normalize();
	//スカラーをかける。
	pos *= 200.0f;
	//プレイヤーの座標に求めたベクトルを足して、カメラの座標とする。
	m_position = m_playerPosition + pos;
	m_position.y += 500.0f;
	g_camera3D.SetTarget(m_target);
	//座標
	g_camera3D.SetPosition(m_position);
	//カメラの更新。
	g_camera3D.Update();
}
//カメラのの状態
void GameCamera::State()
{
	switch (c_state)
	{
	case GameCamera::toPlayer:
		CameraRotate();
		PlayerDamageRot();
		break;
	case GameCamera::toEnemys:
		CameraLookEnemys();
		break;
	}
}
//ダメージを受けたときにカメラが変になる。
void GameCamera::PlayerDamageRot()
{
	if (damage_flag) {
		if (timer == 0.0f) {
			m_right = g_camera3D.GetRight();
		}
		timer += GameTime().GetFrameDeltaTime();
		//時間でざっつに変えてます。
		if (timer < 0.05f)
		{
			m_target += (m_right * 10.0f);
			m_position += (m_right * 10.0f);
		}
		else if (timer >=0.05f && timer <=0.10f)
		{
			m_target -= (m_right * 10.0f);
			m_position -= (m_right * 10.0f);
		}
		else if (timer >= 0.10f && timer <= 0.15f)
		{
			m_target +=(m_right * 10.0f);
			m_position += (m_right * 10.0f);
		}
		else if (timer >= 0.15f && timer <= 0.20f)
		{
			m_target -= (m_right * 10.0f);
			m_position -= (m_right * 10.0f);
		}
		else if (timer > 0.20f)
		{
			//ここで動かなかったところに戻るので
			timer = 0.0f;
			damage_flag = false;
		}
	}
}
//
void GameCamera::OnStartLockOn(Player* pl)
{
	//カメラのターゲットを敵へ。
	c_state = toEnemys;
}
//
void GameCamera::OnEndLockOn(Player* pl)
{
	//カメラのターゲットをプレイヤーへ。
	c_state = toPlayer;
}
//
void GameCamera::OnDamage(Player* pl)
{
	damage_flag = true;
}