#include "stdafx.h"
#include "Golem.h"

//�G������čŏ��ɌĂԏ����B
Golem::Golem()
{
	m_se[0].Init(L"Assets/sound/BossAttack1.wav");						//���̏������B
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
	prm.HP = 100;										//HP
	m_MaxHP = prm.HP;									//MAXHP;
	prm.ATK = 20;										//�U����
	prm.DEF = 30;										//�h���
	prm.SPD = 200;										//�����B
	m_scale = { 3.0f,3.0f,3.0f };						//�傫��
	m_charaCon.Init(50.0f, 200.0f, m_position, enCollisionAttr_Enemy);			//����̑傫���B
	e_state = esIdle;									//�ŏ��ɑҋ@��ԁB
	m_attackEffect = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/Wind.efk");
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
void Golem::Update()
{
	Enemys::Draw();
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
	attackDistance = 400.0f;
	if (m_diff.Length() <= attackDistance && isTracking && fabsf(m_angle) < CMath::PI * 0.20f)
	{
		//�������ɋ߂Â�����U���B
		m_moveSpeed = CVector3::Zero();
		e_state = esStandbyAttack;
	}
}
//�U���܂ł�
void Golem::AttackStanby()
{
	float m_goAttack = 1.0f;
	float LimitTime = 3.0f;
	CVector3 pos = m_position;
	//��x�����v���C���[�܂ł̋�����c�����������߁A�t���O�𗧂Ă܂��B
	if (isDestinationflag) {
		Destination.x = m_player->GetPosition().x - m_position.x;
		Destination.z = m_player->GetPosition().z - m_position.z;
		isDestinationflag = false;
	}
	//���ԂɂȂ�����U���ɑ���B
	if (m_timer >= m_goAttack)
	{
		if (loop) {
			m_playEffectHandle = g_effektEngine->Play(m_attackEffect);
			loop = false;
		}
		m_moveSpeed = Destination * 2.0f;
		m_AttackTime += GameTime().GetFrameDeltaTime();
		e_state = esAttack;
		anim.Play(esAttack);
		g_effektEngine->SetPosition(m_playEffectHandle, pos);
		m_Attackdis = m_position - m_player->GetPosition();
		if (m_Attackdis.Length() <= m_attack)
		{
			m_player->Damage(51);
		}
		if (m_AttackTime >= LimitTime)
		{
			e_state = esAttackGap;
			m_timer = ZERO;
			m_AttackTime = ZERO;
			loop = true;
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
	float StopLimit = 2.0f;
	m_Falltimer += GameTime().GetFrameDeltaTime();
	m_moveSpeed = CVector3::Zero();
	if (m_Falltimer >= StopLimit)
	{
		e_state = esIdle;
		//�����Ńt���O���߂��܂��B
		isDestinationflag = true;
		m_Falltimer = ZERO;
	}
}