#include "stdafx.h"
#include "StoneEnemy.h"


StoneEnemy::StoneEnemy()
{ 
	seModel.Init(L"Assets/modelData/Enemy3.cmo");		//���f���̌Ăяo���B
	//���f���̃A�j���[�V�����̃��[�h�B
	s_animClip[0].Load(L"Assets/animData/E3_idle.tka");	//�A�j���[�V���������[�h�B
	s_animClip[0].SetLoopFlag(true);
	s_animClip[1].Load(L"Assets/animData/E3_walk.tka");	//�A�j���[�V���������[�h�B
	s_animClip[1].SetLoopFlag(true);
	s_animClip[2].Load(L"Assets/animData/E3_ATK.tka");	//�A�j���[�V���������[�h�B
	s_animClip[2].SetLoopFlag(true);
	s_anim.Init(
		seModel,

		s_animClip,
		3
	);
	//�t���O��true��
	//�p�����[�^�[
	prm.HP = 100;										//HP
	prm.ATK = 60;										//�U����
	prm.DEF = 30;										//�h���
	prm.SPD = 400;										//�����B
//	prm.model = seModel;
	m_charaCon.Init(150.0f, 10.0f, m_position);
	e_state = esIdle;
}

StoneEnemy::~StoneEnemy()
{
}

void StoneEnemy::Attack()
{
	m_player->SetDamage(m_ATK);
}

void StoneEnemy::Damage(float Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//�����AHP��0�ȉ��Ȃ玀�S�����B
	if (prm.HP <= 0.0f)
	{
		Death();
	}
}

void StoneEnemy::Search()
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

void StoneEnemy::Update()
{
	Draw();
	EnemyState();
	s_anim.Play(0);
	m_moveSpeed.y -= gravity;
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	seModel.UpdateWorldMatrix(m_position,m_rotation, Scale);
	s_anim.Update(0.05f);
	m_charaCon.SetPosition(m_position);
}

void StoneEnemy::Draw()
{
	seModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void StoneEnemy::Death()
{
	this->SetActive(false);
	m_charaCon.RemoveRigidBoby();
	isDeath = true;
}

void StoneEnemy::EMove() 
{
	Move.Normalize();
	m_moveSpeed = Move * prm.SPD;
}

void StoneEnemy::EnemyState()
{
	switch (e_state)
	{
	case Enemys::esIdle:
		Search();
		s_anim.Play(0);
		break;
	case Enemys::esTracking:
		Search();
		EMove();
		s_anim.Play(1);
		break;
	case Enemys::esAttack:
		break;
	}
}