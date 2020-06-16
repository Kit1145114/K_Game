#include "stdafx.h"
#include "StoneGolem.h"


StoneGolem::StoneGolem()
{
	Model.Init(L"Assets/modelData/RobbotEnemy2.cmo");		//���f���̌Ăяo���B
	//�p�����[�^�[
	animClip[esIdle].Load(L"Assets/animData/RE2_Idle.tka");	//�ҋ@�A�j���[�V���������[�h�B
	animClip[esIdle].SetLoopFlag(true);
	animClip[esTracking].Load(L"Assets/animData/RE2_Move.tka");//�ҋ@�A�j���[�V���������[�h�B
	animClip[esTracking].SetLoopFlag(true);
	animClip[esAttack].Load(L"Assets/animData/RE2_L_Attack.tka");	//�U���A�j���[�V���������[�h�B
	animClip[esAttack].SetLoopFlag(true);
	animClip[esDeath].Load(L"Assets/animData/RE2_Death.tka");	//���ʃA�j���[�V���������[�h�B
	animClip[esDeath].SetLoopFlag(false);
	anim.Init(
		Model,
		animClip,
		m_AnimClipNum
	);
	prm.HP = 100;										//HP
	m_MaxHP = prm.HP;									//MAXHP;
	prm.ATK = 20;										//�U����
	prm.DEF = 30;										//�h���
	prm.SPD = 300;										//�����B
	m_charaCon.Init(25.0f, 75.0f, m_position);		//����̑傫��
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
	Model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
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
	anim.Play(esDeath);
	if (anim.IsPlaying() == false)
	{
		this->SetActive(false);
		m_charaCon.RemoveRigidBoby();
		isDeath = true;
	}
}

void StoneGolem::EnemyState()
{
	switch (e_state)
	{
		//�ҋ@���B
	case Enemys::esIdle:
		Search();
		anim.Play(esIdle);
		break;
		//�ǂ������Ă�B
	case Enemys::esTracking:
		Search();
		EMove();
		anim.Play(esTracking);
		break;
		//�U���B
	case Enemys::esAttack:
		Search();
		anim.Play(esAttack);
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