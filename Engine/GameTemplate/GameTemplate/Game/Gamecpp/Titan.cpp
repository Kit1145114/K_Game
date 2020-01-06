#include "stdafx.h"
#include "Titan.h"


Titan::Titan()
{
	tModel.Init(L"Assets/modelData/RobbotEnemy1.cmo");		//モデルの呼び出し。
//モデルのアニメーションのロード。
	t_animClip[0].Load(L"Assets/animData/RE1_idle.tka");	//アニメーションをロード。
	t_animClip[0].SetLoopFlag(true);
	t_animClip[1].Load(L"Assets/animData/RE1_walk.tka");	//アニメーションをロード。
	t_animClip[1].SetLoopFlag(true);
	t_animClip[2].Load(L"Assets/animData/RE1_death.tka");	//アニメーションをロード。
	t_animClip[2].SetLoopFlag(false);
	t_anim.Init(
		tModel,
		t_animClip,
		3
	);
	//フラグをtrueへ
	//パラメーター
	prm.HP = 100;										//HP
	prm.ATK = 60;										//攻撃力
	prm.DEF = 30;										//防御力
	prm.SPD = 300;										//速さ。
//	prm.model = tModel;
	m_position = e3_pos;
	m_charaCon.Init(150.0f, 10.0f, m_position);
	e_state = esIdle;
}

void Titan::Attack()
{
	//player->SetDamage(m_ATK);
}

void Titan::Damage(float Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//もし、HPが0以下なら死亡処理。
	if (prm.HP <= 0.0f)
	{
		e_state = esDeath;
	}
}

void Titan::Search()
{
	float Track = 0.0f;
	Move = m_player->GetPosition() - m_position;
	if (Move.Length() <= 500.0f)
	{
		e_state = esTracking;
	}
	else if (Move.Length() >= 500.0f)
	{
		e_state = esIdle;
		Move = CVector3::Zero();
	}
}

void Titan::Update()
{
	Draw();
	EnemyState();
	m_moveSpeed.y -= gravity;
	t_anim.Update(0.03f);
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	tModel.UpdateWorldMatrix(m_position, CQuaternion::Identity(), m_scale);
	m_charaCon.SetPosition(m_position);
}

void Titan::Draw()
{
	tModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void Titan::Death()
{
	t_anim.Play(2);
	if (t_anim.IsPlaying() == false)
	{
		this->SetActive(false);
		m_charaCon.RemoveRigidBoby();
		isDeath = true;
	}
}

void Titan::EnemyState()
{
	switch (e_state)
	{
	case Enemys::esIdle:
		Search();
		Rotation();
		t_anim.Play(0);
		break;
	case Enemys::esTracking:
		Search();
		EMove();
		Rotation();
		t_anim.Play(1);
		break;
	case Enemys::esAttack:
		break;
	case Enemys::esDeath:
		Death();
	}
}

void Titan::EMove()
{
	Move.Normalize();
	m_moveSpeed = Move * prm.SPD;
}

void Titan::Rotation()
{
	float None = 0.0f;		//マジックナンバーを防ぐ0を入れた数
	float Rot = atan2(Move.x, Move.z);
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisY(), Rot);
	tModel.SetRotation(qRot);
	//もし、動いていたら回転させる。
	if (m_moveSpeed.x != None || m_moveSpeed.z != None)
	{
		m_rotation = qRot;
		tModel.SetRotation(m_rotation);
	}
	if (m_moveSpeed.x == None && m_moveSpeed.z == None)
	{
		tModel.SetRotation(m_rotation);
	}
	tModel.SetRotation(m_rotation);
}