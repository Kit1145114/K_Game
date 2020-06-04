#include "stdafx.h"
#include "Titan.h"

//�G������čŏ��ɌĂԏ����B
Titan::Titan()
{
	m_se[0].Init(L"Assets/sound/BossAttack1.wav");				//���̏������B
	Model.Init(L"Assets/modelData/RobbotEnemy1.cmo");		//���f���̌Ăяo���B
	//���f���̃A�j���[�V�����̃��[�h�B
	animClip[esIdle].Load(L"Assets/animData/RE1_idle.tka");	//�ҋ@�����[�h�B
	animClip[esIdle].SetLoopFlag(true);
	animClip[esTracking].Load(L"Assets/animData/RE1_walk.tka");	//���������[�h�B
	animClip[esTracking].SetLoopFlag(true);
	animClip[esAttack].Load(L"Assets/animData/RE1_ATK.tka");	//�U�������[�h�B
	animClip[esAttack].SetLoopFlag(true);
	animClip[esDeath].Load(L"Assets/animData/RE1_death.tka");	//���S�����[�h�B
	animClip[esDeath].SetLoopFlag(false);
	anim.Init(
		Model,
		animClip,
		m_AnimClipNum
	);
	anim.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName)
	{
		OnAnimationEvent(clipName, eventName);
	});
	//�t���O��true��
	//�p�����[�^�[
	prm.HP = 100;										//HP
	m_MaxHP = prm.HP;									//MAXHP;
	prm.ATK = 20;										//�U����
	prm.DEF = 30;										//�h���
	prm.SPD = 300;										//�����B
	m_charaCon.Init(50.0f, 150.0f, m_position);		//����̑傫��
	e_state = esIdle;									//�ŏ��Ȃ̂őҋ@�B
}
//�G�̍U�������B
void Titan::Attack()
{
	m_player->Damage(prm.ATK);
}
//DAMAGE�󂯂鏈��
void Titan::Damage(int Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//�����AHP��0�ȉ��Ȃ玀�S�����B
	if (prm.HP <= 0.0f)
	{
		e_state = esDeath;
	}
}
//�v���C���[�̌����鏈���B
void Titan::Search()
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
//�G�̍X�V���e�B
void Titan::Update()
{
	Enemys::Draw();
	Enemys::VectorAcquisition();
	Enemys::Rotation();
	EnemyState();
	m_moveSpeed.y -= gravity;
	anim.Update(0.03f);
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	Model.UpdateWorldMatrix(m_position, m_rotation, {10.0f,10.0f,10.0f});
	m_charaCon.SetPosition(m_position);
}
//�|���ꂽ�Ƃ��ɌĂԏ����B
void Titan::Death()
{
	anim.Play(esDeath);
	if (anim.IsPlaying() == false)
	{
		this->SetActive(false);
		m_charaCon.RemoveRigidBoby();
		isDeath = true;
	}
}
//�G�l�~�[�̃A�j���[�V������Ԃŕς��Ă��
void Titan::EnemyState()
{
	switch (e_state)
	{
		//�ҋ@���B
	case Enemys::esIdle:
		Search();
		Rotation();
		anim.Play(esIdle);
		break;
		//�ǂ������Ă�B
	case Enemys::esTracking:
		Search();
		EMove();
		Rotation();
		anim.Play(esTracking);
		break;
		//�U���B
	case Enemys::esAttack:
		Search();
		Rotation();
		anim.Play(esAttack);
		break;
		//���S�����Ƃ��B
	case Enemys::esDeath:
		Death();
	}
}
//�G�l�~�[���i�ޏ����B
void Titan::EMove()
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
//�A�j���[�V�����C�x���g
void Titan::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (m_player->GetIsDead() == false) {
		if (e_state == esAttack && eventName)
		{
			Attack();
			m_se[0].Play(false);
		}
	}
}
//�U���ł��邩
void Titan::AttackRange()
{
	if (m_diff.Length() <= attackDistance && isTracking)
	{
		//�������ɋ߂Â�����U���B
		e_state = esAttack;
	}
}
