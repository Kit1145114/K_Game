#include "stdafx.h"
#include "Enemys.h"

Enemys::Enemys()
{
	
}

Enemys::~Enemys()
{

}

void Enemys::Init(float HP, float Attack, float Defense, float Speed/*, SkinModel model*/)
{
	m_HP		=	HP;				//ヒットポイントを代入
	m_ATK		=	Attack;			//攻撃力を代入。
	m_DEF		=	Defense;		//防御力を代入。
	m_SPD		=	Speed;			//基礎速度を代入。
	//m_sm		=	model;			//エネミーのモデル。
}
//共通のドロー関数なのでここに記載。
void Enemys::Draw()
{
	Model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		1
	);
}
//共通の視野角
void Enemys::ViewingAngle()
{
	m_diff = m_player->GetPosition() - m_position;
	m_toPlayer = m_player->GetPosition() - m_position;
	m_toPlayer.Normalize();
	m_angle = acosf(m_toPlayer.Dot(m_forward));
}
//共通のベクトルの取得処理。
void Enemys::VectorAcquisition()
{
	auto mRot = CMatrix::Identity();
	mRot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = mRot.m[2][0];
	m_forward.y = mRot.m[2][1];
	m_forward.z = mRot.m[2][2];
}
//共通の回転処理。
void Enemys::Rotation()
{
	float Rot = atan2(Move.x, Move.z);
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisY(), Rot);
	Model.SetRotation(qRot);
	//もし、動いていたら回転させる。
	if (m_moveSpeed.x != ZERO || m_moveSpeed.z != ZERO)
	{
		m_rotation = qRot;
		Model.SetRotation(m_rotation);
	}
	if (m_moveSpeed.x == ZERO && m_moveSpeed.z == ZERO)
	{
		Model.SetRotation(m_rotation);
	}
	Model.SetRotation(m_rotation);
}