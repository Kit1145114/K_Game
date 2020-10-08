#include "stdafx.h"
#include "Titan.h"

//�G������čŏ��ɌĂԏ����B
Titan::Titan()
{
	m_se[0].Init(L"Assets/sound/RE_Attack.wav");			//�U���B
	m_se[1].Init(L"Assets/sound/RE_walk.wav");			//�����B
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
	animClip[esAttackGap].Load(L"Assets/animData/RE1_accumulate.tka");	//���̕��������[�h�B
	animClip[esAttackGap].SetLoopFlag(true);
	animClip[esStandbyAttack].Load(L"Assets/animData/RE1_beforeATK.tka");	//�U���O�̃��[�V���������[�h�B
	animClip[esStandbyAttack].SetLoopFlag(false);
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
	prm.HP = 120;										//HP
	m_MaxHP = prm.HP;									//MAXHP;
	prm.ATK = 55;										//�U����
	prm.DEF = 30;										//�h���
	prm.SPD = 300;										//�����B
	m_charaCon.Init(50.0f, 100.0f, m_position, enCollisionAttr_Enemy);			//����̑傫��
	e_state = Enemys::EnemyAnimState::esIdle;									//�ŏ��Ȃ̂őҋ@�B
	m_attackEffect = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/RobbotEnemyAttack.efk");
}
//�G�������Ƃ��ɏ����B
Titan::~Titan()
{
	g_effektEngine->Stop(m_playEffectHandle);
}
//�U������Ƃ��ɌĂԊ֐�
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
	if (e_state != esAttack) {
		Enemys::ViewingAngle();
		//�̗�MAX��
		if (prm.HP == m_MaxHP && !isTrack_flag) {
			//�͈͊O������p�O�Ȃ炩�A��x�ł������ĂȂ��Ƃ��B
			if (m_diff.Length() >= m_enemyTrack || fabsf(m_angle) > CMath::PI * m_fowndAngle)
			{
				e_state = esIdle;
			}
			//�͈͓�������p���Ȃ�
			else if (m_diff.Length() <= m_enemyTrack && fabsf(m_angle) < CMath::PI * m_fowndAngle)
			{
				m_move = m_player->GetPosition() - m_position;
				isAttack_flag = true;
				e_state = esTracking;
				isTrack_flag = true;
			}
		}
		//�̗͂�MAX����Ȃ��Ƃ��B�Ђ�����ǂ�������B
		else if (prm.HP < m_MaxHP || isTrack_flag)
		{
			m_move = m_player->GetPosition() - m_position;
			isAttack_flag = true;
			isTrack_flag = true;
			e_state = esTracking;
		}
		//�U���͈̔͌v�Z�B
		AttackRange();
	}
}
//�G�̍X�V���e�B
void Titan::Update()
{
	//Enemys::Draw();
	Enemys::VectorAcquisition();
	Enemys::Rotation();
	EnemyState();
	m_moveSpeed.y -= gravity;
	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
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
		m_attackEffect->Release();
		isDeath_flag = true;
		m_se[1].Stop();
	}
}
//�G�l�~�[�̏��
void Titan::EnemyState()
{
	switch (e_state)
	{
		//�ҋ@���B
	case Enemys::esIdle:
		Search();
		anim.Update(GameTime().GetFrameDeltaTime());
		anim.Play(esIdle);
		anim.Update(GameTime().GetFrameDeltaTime());
		break;
		//�ǂ������Ă�B
	case Enemys::esTracking:
		Search();
		EMove();
		anim.Update(GameTime().GetFrameDeltaTime()*1.5f);
		//Rotation();
		anim.Play(esTracking);
		anim.Update(GameTime().GetFrameDeltaTime());
		break;
		//�U���B
	case Enemys::esAttack:
		Enemys::Rotation();
		anim.Update(GameTime().GetFrameDeltaTime());
		anim.Play(esAttack);
		break;
		//���S�����Ƃ��B
	case Enemys::esDeath:
		Death();
		anim.Update(GameTime().GetFrameDeltaTime());
		break;
	case Enemys::esAttackGap:
		AttackCoolTime();
		anim.Update(GameTime().GetFrameDeltaTime());
		break;
	case Enemys::esStandbyAttack:
		Search();
		StandbyAttack();
		anim.Update(GameTime().GetFrameDeltaTime());
		break;
	}
}
//�G�l�~�[���i�ޏ����B
void Titan::EMove()
{
	m_move.Normalize();
	switch (e_state)
	{
	case Enemys::esIdle:
		m_moveSpeed.x = ZERO;
		m_moveSpeed.z = ZERO;
		if (m_se[1].IsPlaying())
		{
			m_se[1].Stop();
		}
		break;
	case Enemys::esAttack:
		m_moveSpeed.x = ZERO;
		m_moveSpeed.z = ZERO;
		if (m_se[1].IsPlaying())
		{
			m_se[1].Stop();
		}
		break;
	case Enemys::esTracking:
		m_moveSpeed = m_move * prm.SPD;
		m_se[1].Play(true);
		break;
	}
}
//�A�j���[�V�����C�x���g
void Titan::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)

{
	if (m_player->GetIsDead() == false) {
		if (e_state == esAttack && eventName && m_diff.Length() <= m_TattackDistance)
		{
			Attack();
			m_se[1].Stop();
			m_se[0].Play(false);
			Enemys::EnemyEffect();
			e_state = esIdle;
		}
		else if (m_diff.Length() >= m_TattackDistance)
		{
			e_state = esIdle;
		}
	}
}
//�U���ł��邩
void Titan::AttackRange()
{
	if (m_diff.Length() <= m_TattackDistance && isAttack_flag
		&& fabsf(m_angle) < CMath::PI * 0.20f)
	{
		//�������ɋ߂Â�����U���B
		e_state = esStandbyAttack;
	}
}
//�U���ҋ@
void Titan::AttackCoolTime()
{
	Enemys::ViewingAngle();
	//�A�j���[�V�����̍Đ��B
	anim.Play(esAttackGap);
	//�U���O�A�����̊ԓ����Ȃ��n�j�q�B
	float m_Limit = 0.75f;
	m_timer += GameTime().GetFrameDeltaTime();
	//���ԂɂȂ�����B
	if (anim.IsPlaying() && m_diff.Length() >= m_TattackDistance)
	{
		e_state = esIdle;
		m_timer = ZERO;
	}

	if (m_timer >= m_Limit)
	{
		//�U��
		e_state = esAttack;
		m_timer = ZERO;
	}
}
//�U������������O�̏���
void Titan::StandbyAttack()
{
	anim.Play(esStandbyAttack);
	//�A�j���[�V�����̍Đ����I�������؂�ւ��B
	if (!anim.IsPlaying())
	{
		e_state = esAttackGap;
	}
	if (m_diff.Length() >= m_Distance)
	{
	e_state = esIdle;
	m_timer = ZERO;
	}
}
