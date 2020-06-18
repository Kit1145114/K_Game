#include "stdafx.h"
#include "StoneGolem.h"


StoneGolem::StoneGolem()
{
	Model.Init(L"Assets/modelData/RobbotEnemy2.cmo");					//���f���̌Ăяo���B
	//�p�����[�^�[
	animClip[esIdle].Load(L"Assets/animData/RE2_Idle.tka");				//�ҋ@�A�j���[�V���������[�h�B
	animClip[esIdle].SetLoopFlag(true);									//�Đ���������̂�true
	animClip[esTracking].Load(L"Assets/animData/RE2_Move.tka");			//�ҋ@�A�j���[�V���������[�h�B
	animClip[esTracking].SetLoopFlag(true);								//�Đ���������̂�true
	animClip[esAttack].Load(L"Assets/animData/RE2_Attack.tka");			//�U���A�j���[�V���������[�h�B
	animClip[esAttack].SetLoopFlag(false);								//�Đ���������̂�true
	animClip[esDeath].Load(L"Assets/animData/RE2_Death.tka");			//���ʃA�j���[�V���������[�h�B
	animClip[esDeath].SetLoopFlag(false);								//���ł����̂�false
	animClip[esAttackGap].Load(L"Assets/animData/RE2_AfterAttack.tka");	//�U���I�������A�j���[�V����
	animClip[esAttackGap].SetLoopFlag(false);							//���ł����̂�false
	anim.Init(
		Model,
		animClip,
		m_AnimClipNum
	);
	prm.HP = 100;										//HP
	m_MaxHP = prm.HP;									//MAXHP;
	prm.ATK = 51;										//�U����
	prm.DEF = 30;										//�h���
	prm.SPD = 300;										//�����B
	m_charaCon.Init(50.0f, 100.0f, m_position, enCollisionAttr_Enemy);		//����̑傫��
	e_state = esIdle;
	m_attackEffect = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/Laser.efk");
}

void StoneGolem::Update()
{
	Enemys::Draw();
	Enemys::VectorAcquisition();
	Enemys::Rotation();
	EnemyState();
	m_moveSpeed.y -= gravity;
	anim.Update(GameTime().GetFrameDeltaTime());
	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	Model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_charaCon.SetPosition(m_position);
}

void StoneGolem::Attack()
{
	anim.Play(esAttack);
	m_timer += GameTime().GetFrameDeltaTime();
	if (m_timer >= 0.5f)
	{
		if (loop) {
			//diff = m_player->GetPosition() - m_position;
			diff.x = m_forward.x;
			diff.z = m_forward.z;
			m_playEffectHandle = g_effektEngine->Play(m_attackEffect);
			g_effektEngine->SetRotation(m_playEffectHandle, ZERO, atan2(diff.x, diff.z), ZERO);
			//�G�t�F�N�g���̓����蔻�萶���B
			CVector3 A = m_position + (m_forward * UpPhyGhostObjPosition * m_objPosAdd);
			A.y += UpPhyGhostObjPosition;
			m_PhyGhostObj.CreateBox(A, m_rotation, box_scale);
			loop = false;
	}
		m_efePos = m_position;
		m_efePos.y += 150.0f;
		//�U���̃G�t�F�N�g
		g_effektEngine->SetPosition(m_playEffectHandle, m_efePos);
		//�U�������Ƃ��̃S�[�X�g���v���C���[�Ɠ������Ă��邩�B
		//HitPlayerObj();
		//�I���܂ł̎��Ԃ��񂷁B
		m_attackTime += GameTime().GetFrameDeltaTime();
		if (m_attackTime >= 3.5f)
		{
			m_PhyGhostObj.Release();
			g_effektEngine->Stop(m_playEffectHandle);
			e_state = esAttackGap;
			m_timer = ZERO;
			m_attackTime = ZERO;
			loop = true;
		}
	}
	//m_player->Damage(prm.ATK);
}

void StoneGolem::AttackRange()
{
	attackDistance = 300.0f;
	if (m_diff.Length() <= attackDistance && isTracking && fabsf(m_angle) < CMath::PI * 0.20f)
	{
		//�������ɋ߂Â�����U���B
		e_state = esAttack;
	}
}

void StoneGolem::Search()
{
	Enemys::ViewingAngle();
	//�̗�MAX��
	m_enemytrack = 1000.0f;
	if (prm.HP == m_MaxHP && !isTrackflag) {
		//�͈͊O������p�O�Ȃ炩�A��x�ł������ĂȂ��Ƃ��B
		if (m_diff.Length() >= m_enemytrack || fabsf(m_angle) > CMath::PI * 0.40f)
		{
			e_state = esIdle;
		}
		//�͈͓�������p���Ȃ�
		else if (m_diff.Length() <= m_enemytrack && fabsf(m_angle) < CMath::PI * 0.40f)
		{
			Move = m_player->GetPosition() - m_position;
			isTracking = true;
			e_state = esTracking;
			isTrackflag = true;
		}
	}
	//�̗͂�MAX����Ȃ��Ƃ��B�Ђ�����ǂ�������B
	else if (prm.HP < m_MaxHP || isTrackflag)
	{
		Move = m_player->GetPosition() - m_position;
		isTracking = true;
		isTrackflag = true;
		e_state = esTracking;
	}
	//�U���͈̔͌v�Z�B
	AttackRange();
}

void StoneGolem::Death()
{
	anim.Play(esDeath);
	g_effektEngine->Stop(m_playEffectHandle);
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
		Attack();
		break;
		//���S�����Ƃ��B
	case Enemys::esDeath:
		Death();
		break;
	case Enemys::esAttackGap:
		AttackAfter();
	}
}
//�L�����̈ړ������B
void StoneGolem::EMove()
{
	Move.Normalize();
	if (e_state == esIdle || e_state == esAttack)
	{
		m_moveSpeed.x = ZERO;
		m_moveSpeed.z = ZERO;
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

void StoneGolem::AttackAfter()
{
	anim.Play(esAttackGap);
	//�I�������ŏ��̏�ԂցB
	if (!anim.IsPlaying())
	{
		e_state = esIdle;
	}
}

void StoneGolem::HitPlayerObj()
{
	//�U���̔���Ƃ��Ă܂��B
	g_physics.ContactTest(m_player->GetCharaCon(), [&](const btCollisionObject& contactObject)
	{
		if (m_PhyGhostObj.IsSelf(contactObject))
		{
			m_player->Damage(prm.ATK);
		}
	});
}