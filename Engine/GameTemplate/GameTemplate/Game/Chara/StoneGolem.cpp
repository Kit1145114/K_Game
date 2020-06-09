#include "stdafx.h"
#include "StoneGolem.h"


StoneGolem::StoneGolem()
{
	Model.Init(L"Assets/modelData/Enemy1.cmo");		//���f���̌Ăяo���B
	//�p�����[�^�[
	prm.HP = 100;										//HP
	m_MaxHP = prm.HP;									//MAXHP;
	prm.ATK = 20;										//�U����
	prm.DEF = 30;										//�h���
	prm.SPD = 300;										//�����B
	m_charaCon.Init(50.0f, 150.0f, m_position);		//����̑傫��
	e_state = esIdle;
}

void StoneGolem::Update()
{
	Enemys::Draw();
	Enemys::VectorAcquisition();
	Enemys::Rotation();
	EnemyState();
	m_moveSpeed.y -= gravity;
	anim.Update(0.03f);
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	Model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());
	m_charaCon.SetPosition(m_position);
}

void StoneGolem::Attack()
{
	m_player->Damage(prm.ATK);
}

void StoneGolem::AttackRange()
{
	if (m_diff.Length() <= attackDistance && isTracking)
	{
		//�������ɋ߂Â�����U���B
		e_state = esAttack;
	}
}

void StoneGolem::Search()
{
	Enemys::ViewingAngle();
	//�̗�MAX��
	if (prm.HP == m_MaxHP) {
		//�͈͊O������p�O�Ȃ�
		if (m_diff.Length() >= m_enemytrack || fabsf(m_angle) > CMath::PI * 0.40f)
		{
			e_state = esIdle;
			isTracking = false;
		}
		//�͈͓�������p���Ȃ�
		else if (m_diff.Length() <= m_enemytrack && fabsf(m_angle) < CMath::PI * 0.40f)
		{
			Move = m_player->GetPosition() - m_position;
			isTracking = true;
			e_state = esTracking;
		}
	}
	//�̗͂�MAX����Ȃ��Ƃ��B�Ђ�����ǂ�������B
	else if (prm.HP < m_MaxHP)
	{
		Move = m_player->GetPosition() - m_position;
		isTracking = true;
		e_state = esTracking;
	}
	//�U���͈̔͌v�Z�B
	AttackRange();
}

void StoneGolem::Death()
{
	this->SetActive(false);
	m_charaCon.RemoveRigidBoby();
	isDeath = true;
}

void StoneGolem::EnemyState()
{
	switch (e_state)
	{
		//�ҋ@���B
	case Enemys::esIdle:
		Search();
		Rotation();
		//anim.Play(esIdle);
		break;
		//�ǂ������Ă�B
	case Enemys::esTracking:
		Search();
		EMove();
		Rotation();
		//anim.Play(esTracking);
		break;
		//�U���B
	case Enemys::esAttack:
		Search();
		Rotation();
		//anim.Play(esAttack);
		break;
		//���S�����Ƃ��B
	case Enemys::esDeath:
		Death();
	}
}

void StoneGolem::EMove()
{
	Move.Normalize();
	if (e_state == esIdle || e_state == esAttack)
	{
		m_moveSpeed.x = 0.0f;
		m_moveSpeed.z = 0.0f;
	}
	else if (e_state == esTracking) {
		m_moveSpeed = Move * prm.SPD;
	}
}

void StoneGolem::HitMe()
{

}

void StoneGolem::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

}

void StoneGolem::Damage(int Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//�����AHP��0�ȉ��Ȃ玀�S�����B
	if (prm.HP <= 0.0f)
	{
		e_state = esDeath;
	}
}