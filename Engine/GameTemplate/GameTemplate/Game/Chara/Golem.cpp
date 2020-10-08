#include "stdafx.h"
#include "Golem.h"

//�G������čŏ��ɌĂԏ����B
Golem::Golem()
{					//���̏������B
	Model.Init(L"Assets/modelData/Enemy2.cmo");							//���f���̏������B
	//���f���̃A�j���[�V�����̃��[�h�B
	animClip[esIdle].Load(L"Assets/animData/E2_idle.tka");				//�ҋ@�A�j���[�V���������[�h�B
	animClip[esIdle].SetLoopFlag(true);									//���[�v����̂�true
	animClip[esTracking].Load(L"Assets/animData/E2_Walk.tka");			//�ҋ@�A�j���[�V���������[�h�B
	animClip[esTracking].SetLoopFlag(true);								//���[�v����̂�true
	animClip[esAttack].Load(L"Assets/animData/E2_ATK.tka");				//�U���A�j���[�V���������[�h�B
	animClip[esAttack].SetLoopFlag(true);								//���[�v����̂�true
	animClip[esDeath].Load(L"Assets/animData/E2_Death.tka");			//���ʃA�j���[�V���������[�h�B
	animClip[esAttackGap].Load(L"Assets/animData/E2_Fall.tka");			//�U���ł��錄�̃A�j���[�V���������[�h
	animClip[esAttackGap].SetLoopFlag(false);							//���[�v���Ȃ��̂�false
	animClip[esStandbyAttack].Load(L"Assets/animData/E2_Standby.tka");	//�U���̂��߂̕��������[�h�B
	animClip[esStandbyAttack].SetLoopFlag(false);						//���[�v���Ȃ��̂�false
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
	prm.HP = 150;										//HP
	m_MaxHP = prm.HP;									//MAXHP;
	prm.ATK = 20;										//�U����
	prm.DEF = 30;										//�h���
	prm.SPD = 200;										//�����B
	m_scale = { 3.0f,3.0f,3.0f };						//�傫��
	m_charaCon.Init(50.0f, 200.0f, m_position, enCollisionAttr_Enemy);			//����̑傫���B
	e_state = esIdle;									//�ŏ��ɑҋ@��ԁB
	m_attackEffect = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/Wind.efk");
	m_se[0].Init(L"Assets/sound/wind.wav");				//�U��
	m_se[1].Init(L"Assets/sound/Enemy2_walk.wav");			//�����B
}
Golem::~Golem()
{
	g_effektEngine->Stop(m_playEffectHandle);
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
			e_state = esTracking;
			isAttack_flag = true;
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
//�G�̍X�V���e�B
void Golem::Update()
{
	Enemys::VectorAcquisition();
	Enemys::Rotation();
	EnemyState();
	m_moveSpeed.y -= gravity;
	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	anim.Update(GameTime().GetFrameDeltaTime());
	m_charaCon.SetPosition(m_position);
	Model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}
//�|���ꂽ�Ƃ��ɌĂԏ����B
void Golem::Death()
{
	anim.Play(esDeath);
	if (m_se[0].IsPlaying())
	{
		m_se[0].Stop();
		g_effektEngine->Stop(m_playEffectHandle);
	}
	if (anim.IsPlaying() == false)
	{
		this->SetActive(false);
		m_charaCon.RemoveRigidBoby();
		isDeath_flag = true;
	}
}
//�G�l�~�[���i�ޏ����B
void Golem::EMove()
{
	m_move.Normalize();
	//��Ԃňړ����邩�����߂�B
	switch (e_state)
	{
	case Enemys::esIdle:
		m_moveSpeed.x = ZERO;
		m_moveSpeed.z = ZERO;
		break;
	case Enemys::esAttack:
		m_moveSpeed.x = ZERO;
		m_moveSpeed.z = ZERO;
		break;
	case Enemys::esTracking:
		m_moveSpeed = m_move * prm.SPD;
		m_se[1].Play(true);
		break;
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
		AttackStanby();
		break;
	case Enemys::esDeath:
		Death();
		break;
	case Enemys::esAttackGap:
		anim.Play(esAttackGap);
		AttackGap();
		break;
	case Enemys::esStandbyAttack:
		AttackStanby();
		break;
	}
}
//�A�j���[�V�����C�x���g
void Golem::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
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
	//��x�U��������A��ɍL�͈͂œ��肷��B
	m_attackDistance = 6000.0f;
	if (m_diff.Length() <= m_attackDistance && isAttack_flag && fabsf(m_angle) < CMath::PI * 0.20f)
	{
		//�������ɋ߂Â�����U���B
		m_moveSpeed = CVector3::Zero();
		e_state = esStandbyAttack;
	}
}
//�U���܂ł�
void Golem::AttackStanby()
{
	CVector3 pos = m_position;
	//��x�����v���C���[�܂ł̋�����c�����������߁A�t���O�𗧂Ă܂��B
	if (isDestination_flag) {
		m_destination.x = m_player->GetPosition().x - m_position.x;
		m_destination.z = m_player->GetPosition().z - m_position.z;
		m_se[1].Stop();
		isDestination_flag = false;
	}
	//���ԂɂȂ�����G�t�F�N�g�̐����B
	if (m_timer >= m_effectSpwanTime)
	{
		//�G�t�F�N�g���ʐ������Ȃ����߂Ɉ�x�����Ăяo���B
		if (isEffectLoop_flag) {
			m_playEffectHandle = g_effektEngine->Play(m_attackEffect);
			m_se[0].Play(false);
			isEffectLoop_flag = false;
		}
		//�ړ����͏�ɍU����Ԃ̂��߁A�G�t�F�N�g����Ǐ]�����Ă���B
		m_moveSpeed = m_destination * 2.0f;
		m_attackTime += GameTime().GetFrameDeltaTime();
		e_state = esAttack;
		anim.Play(esAttack);
		g_effektEngine->SetPosition(m_playEffectHandle, pos);
		m_attackDis = m_position - m_player->GetPosition();
		//�͈͓��ɂ���ԏ�Ƀv���C���[�փ_���[�W�B
		if (m_attackDis.Length() <= m_attackDeistance)
		{
			m_player->Damage(m_playerToDamage);
		}
		//�U������ɔ�����
		if (m_attackTime >= m_goAttackTime)
		{
			e_state = esAttackGap;
			m_timer = ZERO;
			m_attackTime = ZERO;
			isEffectLoop_flag = true;
		}
	}
	else {
		//�^�C�}�[���񂵂܂��B�q�̃^�C�}�[�͓G���U������܂ł̎���
		m_timer += GameTime().GetFrameDeltaTime();
		anim.Play(esStandbyAttack);
	}
}
//�U���ł��錄
void Golem::AttackGap()
{
	//���Ԃ܂œ|��Ă܂�
	g_effektEngine->Stop(m_playEffectHandle);
	m_falltimer += GameTime().GetFrameDeltaTime();
	m_moveSpeed = CVector3::Zero();
	//���ԂɂȂ����炷�ׂĂ����Z�b�g���ē|���B
	if (m_falltimer >= m_fallLimitTime)
	{
		e_state = esIdle;
		//�����Ńt���O���߂��܂��B
		isDestination_flag = true;
		m_falltimer = ZERO;
	}
}