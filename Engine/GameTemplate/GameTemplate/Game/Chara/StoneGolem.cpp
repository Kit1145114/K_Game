#include "stdafx.h"
#include "StoneGolem.h"
#include <complex>


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
	prm.DEF = 70;										//�h���
	prm.SPD = 300;										//�����B
	m_enemyTrack = 1000.0f;								//�̗�MAX��
	m_charaCon.Init(50.0f, 100.0f, m_position, enCollisionAttr_Enemy);		//����̑傫��
	e_state = Enemys::EnemyAnimState::esIdle;
	m_attackEffect = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/Laser2.efk");
	m_se[0].Init(L"Assets/sound/fire4.wav");			//�U��
	m_se[1].Init(L"Assets/sound/laserDamage.wav");		//�_���[�W��
	m_se[2].Init(L"Assets/sound/RE2_Walk.wav");			//�����B
}

StoneGolem::~StoneGolem()
{
	//���񂾌�ɃG�t�F�N�g���c���Ȃ��悤�ɂ���
	g_effektEngine->Stop(m_playEffectHandle);
}
//�G�̍X�V���e�B
void StoneGolem::Update()
{
	//Enemys::Draw();
	Enemys::VectorAcquisition();
	Enemys::Rotation();
	EnemyState();
	m_moveSpeed.y -= gravity;
	anim.Update(GameTime().GetFrameDeltaTime());
	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	Model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_charaCon.SetPosition(m_position);
}
//�U������Ƃ��ɌĂԊ֐�
void StoneGolem::Attack()
{
	anim.Play(esAttack);
	m_timer += GameTime().GetFrameDeltaTime();
	m_se[2].Stop();
	if (m_timer >= 0.5f)
	{
		if (isEffectLoop_flag) {
			diff.x = m_forward.x;
			diff.z = m_forward.z;
			m_playEffectHandle = g_effektEngine->Play(m_attackEffect);
			g_effektEngine->SetRotation(m_playEffectHandle, ZERO, atan2(diff.x, diff.z), ZERO);
			isEffectLoop_flag = false;
		}
		m_se[0].Play(false);
		m_se[0].SetVolume(0.3f);
		m_efePos = m_position;
		m_efePos.y += m_efePosUpY;
		//�U���̃G�t�F�N�g
		g_effektEngine->SetPosition(m_playEffectHandle, m_efePos);
		//�U�������Ƃ��̃S�[�X�g���v���C���[�Ɠ������Ă��邩�B
		HitPlayerObj();
		//�I���܂ł̎��Ԃ��񂷁B
		m_attackTime += GameTime().GetFrameDeltaTime();
		if (m_attackTime >= m_goAttackTime)
		{
			g_effektEngine->Stop(m_playEffectHandle);
			e_state = esAttackGap;
			m_timer = ZERO;
			m_attackTime = ZERO;
			m_se[0].Stop();
			if (m_se[1].IsPlaying())
			{
				m_se[1].Stop();
			}
			isEffectLoop_flag = true;
		}
	}
}
//�U���ł��邩
void StoneGolem::AttackRange()
{
	m_attackDistance = 300.0f;
	if (m_diff.Length() <= m_attackDistance && isAttack_flag && fabsf(m_angle) < CMath::PI * 0.20f)
	{
		//�������ɋ߂Â�����U���B
		e_state = esAttack;
	}
}
//�v���C���[�̌����鏈���B
void StoneGolem::Search()
{
	Enemys::ViewingAngle();
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
//�|���ꂽ�Ƃ��ɌĂԏ����B
void StoneGolem::Death()
{
	anim.Play(esDeath);
	g_effektEngine->Stop(m_playEffectHandle);
	m_PhyGhostObj.Release();
	if (m_se[0].IsPlaying())
	{
		m_se[0].Stop();
		m_attackEffect->Release();
	}
	if (anim.IsPlaying() == false)
	{
		this->SetActive(false);
		m_charaCon.RemoveRigidBoby();
		isDeath_flag = true;
	}
}
//�G�l�~�[�̏��
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
	m_move.Normalize();
	if (e_state == esIdle || e_state == esAttack)
	{
		m_moveSpeed.x = ZERO;
		m_moveSpeed.z = ZERO;
	}
	else if (e_state == esTracking) {
		m_moveSpeed = m_move * prm.SPD;
		m_se[2].SetFrequencyRatio(8.0f);
		m_se[2].Play(true);
	}
}
//DAMAGE�󂯂鏈��
void StoneGolem::Damage(int Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//�����AHP��0�ȉ��Ȃ玀�S�����B
	if (prm.HP <= 0.0f)
	{
		e_state = esDeath;
	}
}
//�U����
void StoneGolem::AttackAfter()
{
	anim.Play(esAttackGap);
	//�I�������ŏ��̏�ԂցB
	if (!anim.IsPlaying())
	{
		e_state = esIdle;
	}
}
//�G�t�F�N�g���v���C���[�ɓ������Ă��邩�̏����B
void StoneGolem::HitPlayerObj()
{
	//�U���̔���Ƃ��Ă܂��B
	//A�x�N�g��(�P�ʃx�N�g��)�A�����K���̐��ʂ̃x�N�g��
	CVector3 front = m_forward;
	CVector3 pos = m_player->GetPosition() - m_position;
	//A�x�N�g����B�x�N�g���̓��ς����߂�
	//����ɂ��uB�x�N�g������A�x�N�g���ɐ����x�N�g���v�ƁuA�x�N�g���v�̌������W����A�x�N�g���̎n�_�܂ł̒��������܂�
	float ip = front.Dot(pos);
	if (ip > 0 && ip < m_length) {
		//A�x�N�g���ɋ��߂����ς������āA�uB�x�N�g������A�x�N�g���ɐ����x�N�g���v�ƁuA�x�N�g���v�̌������W��C�x�N�g�������߂�
		CVector3 c = front * ip;
		//C�x�N�g����B�x�N�g���̋����̋�����2�悪���ȉ�(�G�̃R���W�����̔��a�{�����K���̂��񂿂��̔��a��2��)�Ȃ�v���C���[�Ƀ_���[�W��^����
		CVector3 pos2 = pos - c;
		if (pos2.LengthSq() <= std::pow((m_player->GetRadius() + m_r), 2.0f)) {
			m_player->Damage(prm.ATK);
			if (SoundMakeFlag)
			{
				m_se[1].Play(false);
				m_se[1].SetVolume(0.3f);
				SoundMakeFlag = false;
			}
		}
		else if(m_se[1].IsPlaying() &&  e_state != esAttack
			|| pos2.LengthSq() >= std::pow((m_player->GetRadius() + m_r), 2.0f))
		{
			m_se[1].Stop();
			SoundMakeFlag = true;
		};
	}
}