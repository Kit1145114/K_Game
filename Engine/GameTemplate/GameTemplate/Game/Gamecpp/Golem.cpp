#include "stdafx.h"
#include "Golem.h"


Golem::Golem()
{
	gModel.Init(L"Assets/modelData/Enemy2.cmo");		//モデルの呼び出し。
//モデルのアニメーションのロード。
	g_animClip[0].Load(L"Assets/animData/E2_idle.tka");	//待機アニメーションをロード。
	g_animClip[0].SetLoopFlag(true);
	g_animClip[1].Load(L"Assets/animData/E2_Death.tka");//死ぬアニメーションをロード。
	g_animClip[1].SetLoopFlag(false);
	g_animClip[2].Load(L"Assets/animData/E2_ATK.tka");	//攻撃アニメーションをロード。
	g_animClip[2].SetLoopFlag(true);
	g_anim.Init(
		gModel,
		g_animClip,
		3
	);
	//フラグをtrueへ
	//パラメーター
	prm.HP = 100;										//HP
	prm.ATK = 60;										//攻撃力
	prm.DEF = 30;										//防御力
	prm.SPD = 200;										//速さ。
//	prm.model = gModel;
	m_position = e2_pos;
	m_charaCon.Init(150.0f, 10.0f, m_position);
	e_state = esIdle;
}

void Golem::Attack()
{
	m_player->SetDamage(m_ATK);
}

void Golem::Damage(float Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//もし、HPが0以下なら死亡処理。
	if (prm.HP <= 0.0f)
	{
		e_state = esDeath;
	}
}

void Golem::Search()
{
	float Track = 0.0f;
	Move = m_player->GetPosition() - m_position;
	if (Move.Length() <= 1500.0f)
	{
		e_state = esTracking;
	}
	else if (Move.Length() >= 1500.0f)
	{
		e_state = esIdle;
		Move = CVector3::Zero();
	}
}

void Golem::Update()
{
	Draw();
	EnemyState();
	m_moveSpeed.y -= gravity;
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	g_anim.Update(0.05f);
	m_charaCon.SetPosition(m_position);
	gModel.UpdateWorldMatrix(m_position, CQuaternion::Identity(), scale);
}

void Golem::Draw()
{
	gModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void Golem::Death()
{
	g_anim.Play(1);
	if (g_anim.IsPlaying() == false)
	{
		this->SetActive(false);
		m_charaCon.RemoveRigidBoby();
		isDeath = true;
	}
}

void Golem::EMove()
{
	Move.Normalize();
	m_moveSpeed = Move * prm.SPD;
}

void Golem::EnemyState()
{
	switch (e_state)
	{
	case Enemys::esIdle:
		Search();
		g_anim.Play(0);
		break;
	case Enemys::esTracking:
		Search();
		EMove();
		g_anim.Play(0);
		break;
	case Enemys::esAttack:
		g_anim.Play(2);
		break;
	case Enemys::esDeath:
		Death();
	}
}
