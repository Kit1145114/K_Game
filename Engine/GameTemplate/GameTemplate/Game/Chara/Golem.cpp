#include "stdafx.h"
#include "Golem.h"

//�G������čŏ��ɌĂԏ����B
Golem::Golem()
{
	m_se[0].Init(L"Assets/sound/BossAttack1.wav");				//���̏������B
	Model.Init(L"Assets/modelData/Enemy2.cmo");				//���f���̏������B
//���f���̃A�j���[�V�����̃��[�h�B
	animClip[esIdle].Load(L"Assets/animData/E2_idle.tka");	//�ҋ@�A�j���[�V���������[�h�B
	animClip[esIdle].SetLoopFlag(true);
	animClip[esTracking].Load(L"Assets/animData/E2_Walk.tka");//�ҋ@�A�j���[�V���������[�h�B
	animClip[esTracking].SetLoopFlag(true);
	animClip[esAttack].Load(L"Assets/animData/E2_ATK.tka");	//�U���A�j���[�V���������[�h�B
	animClip[esAttack].SetLoopFlag(true);
	animClip[esDeath].Load(L"Assets/animData/E2_Death.tka");	//���ʃA�j���[�V���������[�h�B
	animClip[esDeath].SetLoopFlag(false);
	anim.Init(
		Model,
		animClip,
		m_AnimClipNum
	);
	//�A�j���[�V�����C�x���g���ĂԁB
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
	prm.SPD = 200;										//�����B
	m_scale = { 5.0f,5.0f,5.0f };							//�傫��
	m_position = e_pos1;
	m_charaCon.Init(150.0f, 500.0f, m_position);			//����̑傫���B
	e_state = esIdle;									//�ŏ��ɑҋ@��ԁB
}
//�G�̍U�������B
void Golem::Attack()
{
	m_player->Damage(prm.ATK);
}
//DAMAGE�󂯂鏈��
void Golem::Damage(int Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//�����AHP��0�ȉ��Ȃ玀�S�����B
	if (prm.HP <= 0.0f)
	{
		e_state = esDeath;
	}
}
//�v���C���[�̌����鏈���B
void Golem::Search()
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
void Golem::Update()
{
	Enemys::Draw();
	Enemys::VectorAcquisition();
	Enemys::Rotation();
	EnemyState();
	m_moveSpeed.y -= gravity;
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	anim.Update(0.05f);
	m_charaCon.SetPosition(m_position);
	Model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}
//�|���ꂽ�Ƃ��ɌĂԏ����B
void Golem::Death()
{
	anim.Play(esDeath);
	if (anim.IsPlaying() == false)
	{
		this->SetActive(false);
		m_charaCon.RemoveRigidBoby();
		isDeath = true;
	}
}
//�G�l�~�[���i�ޏ����B
void Golem::EMove()
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
//�G�l�~�[�̃A�j���[�V������Ԃŕς��Ă��
void Golem::EnemyState()
{
	switch (e_state)
	{
	case Enemys::esIdle:
		Search();
		anim.Play(esIdle);
		break;
	case Enemys::esTracking:
		Search();
		EMove();
		anim.Play(esTracking);
		break;
	case Enemys::esAttack:
		Search();
		EMove();
		anim.Play(esAttack);
		break;
	case Enemys::esDeath:
		Death();
	}
}
//�A�j���[�V�����C�x���g
void Golem::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//if (m_player->GetIsDead() == false) {
	//	if (e_state == esAttack && eventName)
	//	{
	//		Attack();
	//		m_se[0].Play(false);
	//	}
	//}
	if (m_player->GetIsDead() == false) {
		CVector3 diff = m_position - m_player->GetPosition();
		if (diff.Length() <= m_Kyori && eventName)
		{
			Attack();
		}
	}
}
//�U���ł��邩
void Golem::AttackRange()
{
	if (m_diff.Length() <= attackDistance && isTracking)
	{
		//�������ɋ߂Â�����U���B
		e_state = esAttack;
	}
}