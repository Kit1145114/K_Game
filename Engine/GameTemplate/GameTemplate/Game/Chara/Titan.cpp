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
	e_state = esIdle;									//�ŏ��Ȃ̂őҋ@�B
	m_attackEffect = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/RobbotEnemyAttack.efk");

}
//�G�������Ƃ��ɏ����B
Titan::~Titan()
{
	g_effektEngine->Stop(m_playEffectHandle);
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
		isDeath = true;
		m_se[1].Stop();
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
		anim.Update(GameTime().GetFrameDeltaTime());
		//Rotation();
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
		break;
		//�U���B
	case Enemys::esAttack:
		Search();
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
		if (m_se[1].IsPlaying())
		{
			m_se[1].Stop();
		}
	}
	else if (e_state == esTracking) {
		m_moveSpeed = Move * prm.SPD;
		m_se[1].Play(true);
	}
}
//�A�j���[�V�����C�x���g
void Titan::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	CVector3 dis;
	CVector3 diff;
	if (m_player->GetIsDead() == false) {
		if (e_state == esAttack && eventName && m_diff.Length() <= m_TattackDistance)
		{
			Attack();
			m_se[1].Stop();
			m_se[0].Play(false);
			dis = (m_player->GetPosition() + m_position) * 0.5f;
			diff = m_player->GetPosition() - m_position;
			dis.y += 30.0f;
			//{ m_forward.x, 50.0f,m_forward.z };
			m_playEffectHandle = g_effektEngine->Play(m_attackEffect);
			g_effektEngine->SetPosition(m_playEffectHandle,/*m_player->GetPosition()*/dis);
			g_effektEngine->SetRotation(m_playEffectHandle, 0.0f, atan2( diff.x, diff.z), 0.0f);
			e_state = esAttackGap;
		}
		else if (e_state == esAttack && eventName && m_diff.Length() >= m_TattackDistance)
		{
			e_state = esAttackGap;
		}
	}
}
//�U���ł��邩
void Titan::AttackRange()
{
	if (m_diff.Length() <= attackDistance && isTracking && fabsf(m_angle) < CMath::PI * 0.20f)
	{
		//�������ɋ߂Â�����U���B
		e_state = esAttack;
	}
}
//�U���ҋ@
void Titan::AttackCoolTime()
{
	//�U����A�����̊ԓ����Ȃ��n�j�q�B
	float Limit = 0.75f;
	if (e_state == esAttackGap)
	{
		anim.Play(esIdle);
		m_timer += GameTime().GetFrameDeltaTime();
	}
	//���ԂŖ߂�B
	if (m_timer > Limit)
	{
		e_state = esIdle;
		m_timer = ZERO;
	}
}
