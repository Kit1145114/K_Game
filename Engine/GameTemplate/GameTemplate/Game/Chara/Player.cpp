#include "stdafx.h"
#include "Player.h"
#include"../GameSystem/GameConst.h"
#include"Game/Game.h"
#define _USE_MATH_DEFINES //M_PI �~�����Ăяo��
#include <math.h> 
#include "graphics/shadow/ShadowMap.h"
#include"../GameSystem/GameCamera.h"

Player::Player()
{
	//���ʉ�
	m_se[0].Init(L"Assets/sound/enemy_attack_00.wav");
	m_se[1].Init(L"Assets/sound/Boost.wav");
	m_se[2].Init(L"Assets/sound/swing.wav");
	m_se[3].Init(L"Assets/sound/PlayerWalk.wav");
	//Assets/sound/PlayerWalk.wav
	//cmo�t�@�C���̓ǂݍ��݁B
	Gmodel.Init(L"Assets/modelData/Player.cmo");		//�v���C���[�̕`��
	g_animClip[pl_idle].Load(L"Assets/animData/P_idle.tka");	//�ҋ@�̃��[�h
	g_animClip[pl_idle].SetLoopFlag(true);
	g_animClip[pl_Walk].Load(L"Assets/animData/P_walk.tka");	//�����̃��[�h
	g_animClip[pl_Walk].SetLoopFlag(true);
	g_animClip[pl_FlyMove].Load(L"Assets/animData/P_FlyMove.tka");	//�u�[�X�g�̃��[�h
	g_animClip[pl_FlyMove].SetLoopFlag(true);
	g_animClip[pl_Atk].Load(L"Assets/animData/P_ATK.tka");	//����̃��[�h
	g_animClip[pl_Atk].SetLoopFlag(false);
	g_animClip[pl_Combo].Load(L"Assets/animData/PlayerCombo.tka");	//����̃��[�h
	g_animClip[pl_Combo].SetLoopFlag(false);
	g_anim.Init(
		Gmodel,			//�A�j���[�V�����𗬂��X�L�����f��
						//����ŃA�j���[�V�����ƃX�L�����f�����֘A�t������B
		g_animClip,		//�A�j���[�V�����N���b�v�̔z��B
		m_AnimClipNum		//�A�j���[�V�����N���b�v�̐�
	);
	//�悵�A���Ⴀ�A�j���[�V�����C�x���g�Ԃ�����ł�邺�B
	g_anim.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName)
	{
		OnAnimationEvent(clipName, eventName);
	});

	m_charaCon.Init(60.0f, 120.0f, m_position, enCollisionAttr_Character);			//�L�����R���̐ݒ�i���a�A�����A�����ʒu�B�j
	HP = 100.0f;		//�v���C���[�̏����̗́B
	MaxHp = HP;			//�ő�l�̍X�V�B
	ATK = 100;		//�v���C���[�̍U���́B
	DEF = 50;		//�v���C���[�̖h��́B
	ENERGY = 300.0f;	//�v���C���[�̃u�[�X�g�ʁB
	playerState = pl_idle;
	playerENE = ene_Full;
	m_attackEffect[0] = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/test.efk");
	m_attackEffect[1] = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/hit.efk");
	m_attackEffect[2] = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/Boost.efk");

	TextureData textureData;
	textureData.emissionFilePath = L"Resource/sprite/emission.dds";
	Gmodel.InitTexture(&textureData);
}

Player::~Player()
{
}
//�v���C���[�̃A�b�v�f�[�g�B
void Player::Update()
{
	//�v���C���[�̍X�V�������ɋL�q�B
	if (playerState != pl_Death) {
		Energy();
		Move();							//�v���C���[�̈ړ����ĂԁB
		PlayerAttack();					//�v���C���[�̍U����
		PlayerState();					//�v���C���[�̏�Ԃ��ĂԁB
		Rotation();						//�v���C���[�̉�]���ĂԁB
		Forward();						//�v���C���[�̑O�x�N�g���擾�B
		RookOnEnemys();					//�G�l�~�[���^�[�Q�e�B���O����p�B
	}
	g_anim.Update(GameTime().GetFrameDeltaTime() * NSpeed);	//�A�j���[�V�������t���[���P�ʂŕ`��B
			//���[���h�s��̍X�V�B
	Gmodel.UpdateWorldMatrix(m_position, m_rotation, {0.5f,0.5f,0.5f});

	g_graphicsEngine->GetShadowMap()->UpdateFromLightTarget(CVector3(m_position.x + 1000.0f, m_position.y + 1000.0f, m_position.z + 1000.0f),m_position);
}
//�v���C���[�̕`�揈���B
void Player::Draw()
{
	Gmodel.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix(),
		1
	);
}
//�v���C���[��HP�\��
void Player::FontRender()
{
	//�ꏊ�ƐF�̎w��B
	m_font.DrawScreenPos(L"Player", CVector2(190, 40),
		CVector4(0.0f,1.0f,0.0f,1.0f));
	wchar_t text[10];
	swprintf_s(text, L"%d",HP);
	m_font.DrawScreenPos(text, CVector2(400, 40),
		CVector4(0.0f, 1.0f, 0.0f, 1.0f));
}
//�v���C���[�̈ړ�����
void Player::Move()
{
	//�i�ޑ����̒l���߁B
	float Speed = 450.0f;
	//���X�e�B�b�N�󂯎��}�V�[��
	float lStick_x = (g_pad[0].GetLStickXF());
	float lStick_y = (g_pad[0].GetLStickYF());
	//�J�����̑O�������ƉE�������擾�B
	cameraForward = g_camera3D.GetForward();
	cameraRight = g_camera3D.GetRight();
	//XZ���ʂł̑O�������A�E�����ɕϊ�����B
	cameraForward.y = None;
	cameraForward.Normalize();
	cameraRight.y = None;
	cameraRight.Normalize();
	//XZ�����̈ړ����x���N���A�B
	m_moveSpeed.x = None;
	m_moveSpeed.z = None;
	if (playerState != pl_Atk ) {
		if (g_pad[0].IsPress(enButtonX) && playerENE == ene_Full) {
			//����
			m_moveSpeed += cameraForward * lStick_y * Speed * RunSPeed;		//�������ւ̈ړ����x�����Z�B
			m_moveSpeed += cameraRight * lStick_x * Speed * RunSPeed;		//�E�����ւ̈ړ����x�����Z�B
			SoundTrue(1);
		}
		else if (!g_pad[0].IsPress(enButtonX) || playerENE == ene_Charge)
		{
			//�����B
			m_moveSpeed += cameraForward * lStick_y * Speed * NSpeed;	//�������ւ̈ړ����x�����Z�B
			m_moveSpeed += cameraRight * lStick_x * Speed * NSpeed;		//�E�����ւ̈ړ����x�����Z�B
			g_effektEngine->Stop(m_playEffectHandle);
		}
	}
	else if (playerState == pl_Atk)
	{
		m_moveSpeed.z += m_forward.z * AttackMoveSpeed;		//�O������ւ̈ړ����x�����Z�B
		m_moveSpeed.x += m_forward.x * AttackMoveSpeed;		//���E�����ւ̈ړ����x�����Z�B
		
	}
	//�L�����N�^�[�R���g���[���[�ɂP�t���[���̌o�ߕb���A���ԃx�[�X�̈ړ����x��n���Ă���B
	//�L�����R���ɍ��W��n���B
	m_charaCon.SetPosition(m_position);	
	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
}
//�v���C���[�̉�]����
void Player::Rotation()
{
	CQuaternion qRot;
	//�ړ����������ɉ�]������B
	m_Rot = atan2(m_moveSpeed.x, m_moveSpeed.z);
	qRot.SetRotation(CVector3::AxisY(), m_Rot);
	Gmodel.SetRotation(qRot);
	//�����A�����Ă������]������B
	if (m_moveSpeed.x != None || m_moveSpeed.z != None)
	{
		m_rotation = qRot;
		Gmodel.SetRotation(m_rotation);
	}
	else if (m_moveSpeed.x == None || m_moveSpeed.z == None)
	{
		Gmodel.SetRotation(m_rotation);
	}
	Gmodel.SetRotation(m_rotation);
}
//�v���C���[�̏�ԏ����B
void Player::PlayerState()
{
	//�����A�����Ă��Ȃ��Ƃ�����ł킩��₷���������ɕύX�\��B
	//�v���C���[�̏�Ԃŏ����̕ύX�B
	switch (playerState)
	{
	case pl_idle:	//�ҋ@���
		MoveOperation();
		g_anim.Play(pl_idle, 0.1f);
		StopSound(1);
		break;
	case pl_Walk:	//������ԁB
		MoveOperation();
		g_anim.Play(pl_Walk, 0.1f);
		StopSound(1);
		break;
	case pl_FlyMove:
		MoveOperation();
		g_anim.Play(pl_FlyMove, 0.1f);
		SoundTrue(1);
		break;
	case pl_Atk:	//�U����ԁB
		ComboAttack();
		toEnemyInduction();				//�Ǐ]
		break;
	case pl_Death:	//���S���
		g_anim.Play(0);
		break;
	}
}
//�v���C���[�̍U���ށB
void Player::PlayerAttack()
{
	//����B�{�^���������ꂽ��A�p���`�B
	//�U�����[�V�����ƃS�[�X�g�̓����蔻��𐶐��B
	if (m_isCombo_flag && g_pad[0].IsTrigger(enButtonY))
	{
		m_ComboNow_flag = true;
	}
	else if (g_pad[0].IsTrigger(enButtonY))
	{
		toEnemyInduction();				//�Ǐ]
		playerState = pl_Atk;
		m_isCombo_flag = true;
	}
}
//�v���C���[�̈ړ��ށB
void Player::MoveOperation()
{
		//�p�b�h��ABUTTON���͂ŏ㏸����B
		if (g_pad[0].IsPress(enButtonA) && playerENE == ene_Full)
		{
			m_moveSpeed.y += JumpPower;
		}
		else
		{
			//�܂��㏸�������̃X�s�[�h���[���ցB
			if (!g_pad[0].IsPress(enButtonA) && m_moveSpeed.y >= ZERO) {
				//Y�����ւ̏d�͂�����B
				m_moveSpeed.y = ZERO; //* (1.0f / 60.0f);
			}
			//�����ŗ���
			if (!g_pad[0].IsPress(enButtonA) || playerENE == ene_Charge)
			{
				m_moveSpeed.y -= gravity * 1.5;
			}
		}
		//�����ĂȂ��Ƃ��͑ҋ@�ɁB
		if (m_moveSpeed.x == ZERO && m_moveSpeed.z == ZERO)
		{
			playerState = pl_idle;
		}
		//���V�ړ��Ƃ��B
		else if (g_pad[0].IsPress(enButtonX) && playerENE == ene_Full)
		{
			playerState = pl_FlyMove;
		}
		//�����̎��B
		else if (!g_pad[0].IsPress(enButtonX) || playerENE == ene_Charge)
		{
			playerState = pl_Walk;
		}
}
//�A�j���[�V�����C�x���g
void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�S�[�X�g�I�u�W�F�N�g�����ʒu�����߂Ă��܂��B
	boxPos = m_position + (m_forward * UpPhyGhostObjPosition);
	boxPos.y += UpPhyGhostObjPosition;
	float m_efkPosUpY = 65.0f;
	//�A�j���[�V�����C�x���g���Ă΂ꂽ�Ƃ��ɃS�[�X�g�I�u�W�F�N�g�𐶐��B
	m_PhyGhostObj.CreateBox(boxPos, m_rotation, box_scale);
	for (auto& listener : m_eventListenerList) {
		listener->OnOccurredAttackCollision(m_PhyGhostObj,eventName,ATK);
	}

	//�폜�B
	m_PhyGhostObj.Release();
}
//�O�x�N�g��
void Player::Forward()
{
	auto mRot = CMatrix::Identity();
	mRot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = mRot.m[2][0];
	m_forward.y = mRot.m[2][1];
	m_forward.z = mRot.m[2][2];
}
//DAMAGE�̏����B
void Player::Damage(int Damage)
{
	//�_���[�W��0s�ȉ��Ȃ�󂯂Ȃ�
	if((Damage - DEF) >= ZERO)
	//�_���[�W��H�����...�B
	HP -= (Damage - DEF);
	//���X�i�[�Ƀ_���[�W���󂯂����Ƃ�ʒm����B
	for (auto& Listener : m_eventListenerList)
	{
		Listener->OnDamage(this);
	}
	//�����AHP��0�ȉ��Ȃ玀�S�����B
	if (HP <= 0.0f)
	{
		m_isdeath_flag = true;
	}
	//You are Dead�B
	if (m_isdeath_flag)
	{
		playerState = pl_Death;
		//�v���C���[�����S�������Ƃ��C�x���g���X�i�[�ɒʒm�B
		NotifyDeadEventToListener();
	}
}
//���X�i�[�Ƀv���C���[�����񂾂��Ƃ�ʒm���鏈���B
void Player::NotifyDeadEventToListener()
{
	//�C�x���g���X�i�[�Ɏ��S��ʒm����B
	for (auto& listener : m_eventListenerList) {
		listener->OnPlayerDead(this);
	}
}
//�G�l���M�[�Ɋւ��鏈���B
void Player::Energy()
{
	//�G�i�W�[��0�ɂȂ����烂�[�h�ύX�B
	if (ENERGY <= MINENERGY)
	{
		playerENE = ene_Charge;
	}
	//charge���[�h���G�i�W�[���}�b�N�X�ɂȂ�܂�
	if (ENERGY < MAXENERGY && playerENE == ene_Charge)
	{
		ENERGY += ENERGYFLUCT / 3;
	}
	//�G�i�W�[���t���ɂȂ�����ʏ탂�[�h
	if (ENERGY >= MAXENERGY)
	{
		playerENE = ene_Full;
	}
	if (ENERGY > MINENERGY && playerENE == ene_Full)
	{
		//MIN�K��l�ȉ������V�ړ����B
		if (playerState == pl_FlyMove)
		{
			ENERGY -= ENERGYFLUCT;
		}
		else if (g_pad[0].IsPress(enButtonA))
		{
			ENERGY -= ENERGYFLUCT/1.5f;
		}
		//MAX�K��l�ȉ����ҋ@���B
		else if (playerState == pl_idle && ENERGY < MAXENERGY && !g_pad[0].IsPress(enButtonA))
		{
			ENERGY += ENERGYFLUCT;
		}
		//MAX�K��l�ȉ������������n��ɂ���Ƃ��B
		else if (playerState == pl_Walk && ENERGY < MAXENERGY && !g_pad[0].IsPress(enButtonA))
		{
			ENERGY += ENERGYFLUCT;
		}
	}

}
//�G�����b�N�I�����邽�߂̏����B
void Player::RookOnEnemys()
{
	//���݂̃��b�N�I����Ԃ��L�^����B
	bool isOldLockOnFlag = m_isLockOn_flag;
	//�v���C���[�̌����Ă���p�x�̌v�Z�B
	float degreep = atan2(m_forward.x, m_forward.z);
	//�v�Z���ďo���b��I�Ɉ�ԏ������p�x���L������ϐ��B
	float degreemum = M_PI * 2;
	//�͈͊O�B
	float distance = 2000.0f;
	//��ԋ߂��G�l�~�[�Ƃ̋����B�����l�͑傫�Ȑ��l�ɂ��Ă����B
	float distanceNearEnemy = 10000000;
	//���b�N�I���\�t���O
	bool isPossibleLockOn = false;
	for (Enemys* enemys : m_enemysList)
	{
		if (enemys->GetisDeath())
			continue;
		CVector3 dist = m_position - enemys->GetPosition();
		//�@���_���璍���_�Ɍ������ĐL�т�x�N�g��V1���v�Z����B
		CVector3 m_cameraToPlayer = g_Camera->GetPosition() - m_position;
		//�A���_����G�l�~�[�Ɍ������ĐL�т�x�N�g��V2���v�Z����B
		CVector3 m_cameraToEnemy = g_Camera->GetPosition() - enemys->GetPosition();
		//�BV1�𐳋K������B
		m_cameraToPlayer.Normalize();
		//�CV2�𐳋K������B
		m_cameraToEnemy.Normalize();
		//�DV1��V2�̓��ό���t�����߂�B
		float t = m_cameraToPlayer.Dot(m_cameraToEnemy);


		float distEnemy = dist.Length();
		if (distEnemy < distanceNearEnemy
			&& distEnemy < distance
			&& t >= ZERO  
		) {
			//�����̂ق����߂��B
			//��ԋ߂��G�l�~�[�̋������X�V����B
			distanceNearEnemy = distEnemy;
			m_enemyPos = enemys->GetPosition();
			//���b�N�I���\�ɂ���B
			isPossibleLockOn = true;
		}

	}	
	//���߂���ԏ������l�����l��菬�����ꍇ�A�^�[�Q�e�B���O���I���ɂ���B
	if (m_isLockOn_flag) {
		if (g_pad[0].IsTrigger(enButtonB)) {
			m_isLockOn_flag = false;
		}
	}
	else {
		if (isPossibleLockOn) {
			if (g_pad[0].IsTrigger(enButtonB)) {
				m_isLockOn_flag = true;
			}
		}
	}

	//���b�N�I���̐؂�ւ������X�i�[�ɒʒm�B
	TryNotifyChangeLockonEventToListener(isOldLockOnFlag);
}
//�v���C���[���J�����̒����_��ύX�����Ƃ��ɌĂԏ���
void Player::TryNotifyChangeLockonEventToListener(bool isOldLockOnFlag)
{
	if (isOldLockOnFlag != m_isLockOn_flag) {
		//���b�N�I����Ԃ��ς�����̂ŁA���X�i�[�ɒʒm�B
		if (m_isLockOn_flag) {
			//���b�N�I����Ԃɐ؂�ւ�����̂����X�i�[�ɒʒm�B
			for (auto& listener : m_eventListenerList) {
				listener->OnStartLockOn(this);
			}
		}
		else {
			//���b�N�I����Ԃ��I������̂����X�i�[�ɒʒm�B
			for (auto& listener : m_eventListenerList) {
				listener->OnEndLockOn(this);
			}
		}
	}
}
//�U�����ɍēx���͂��邱�ƂŁA2���ڂ̍U�����ł܂��B
void Player::ComboAttack()
{
	//�R���{����Ȃ��Ƃ��͈��
	if (!m_ComboNow_flag)
	{
		g_anim.Play(pl_Atk,0.1f);

		//�U�����I�������B
		if (!g_anim.IsPlaying())
		{
			ComboReset();
		}
	}
	//�R���{���͓��
	else if (m_ComboNow_flag&& !g_anim.IsPlaying())
	{
		g_anim.Play(pl_Combo,0.1f);
		//�U�����I�������B
		if (!g_anim.IsPlaying())
		{
			ComboReset();
		}
	}
}
//�񕜂���Ƃ��̋L�q
void Player::PlayerHeal(int healPoint)
{
	HP += healPoint;
	//HP����������Ȃ��悤�ɁB
	if (HP > MaxHp)
	{
		HP = MaxHp;
	}
}
//�G�̕����Ɍ����鏈���B
void Player::toEnemyInduction()
{
	float m_distance = 50.0f;
	//���X�g����G�S���̈ʒu���g���܂��B
	for (auto enemy : m_enemysList)
	{
		if (!enemy->GetIsDead()) {
			toEnemy = enemy->GetPosition() - m_position;
			Angle();
		}
		//���������p�x���B
		if (toNearEnemyPos.Length() <= m_distance
			&& fabsf(m_angle) < CMath::PI * RookAngle)
		{
			//�v�Z�A����B�����ă��f���ɃZ�b�g�B
			PlayerRot = atan2(toNearEnemyPos.x, toNearEnemyPos.z);
			m_rotation.SetRotation(CVector3::AxisY(), PlayerRot);
			Gmodel.SetRotation(m_rotation);
			isDiscoveryFlag = true;
		}
		else if(toNearEnemyPos.Length() >= m_distance
			&& fabsf(m_angle) > CMath::PI* RookAngle)
		{
			//�͈͂ɂ��Ȃ��Ȃ炻�̏ꏊ�ōU�����[�V�����B
			m_rotation.SetRotation(CVector3::AxisY(),m_Rot);
			Gmodel.SetRotation(m_rotation);
			isDiscoveryFlag = false;
		}
	}
}
//�A���O���B
void Player::Angle()
{
	//��ԋ߂��G�̃|�W�V�������v�Z�B
	if (isDiscoveryFlag &&toEnemy.Length() < NearEnemys)
	{
		toNearEnemyPos = toEnemy;
	}
	toNearEnemyPos.Normalize();
	m_angle = acosf(toNearEnemyPos.Dot(m_forward));
}