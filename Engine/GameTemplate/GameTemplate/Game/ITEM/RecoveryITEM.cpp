#include "stdafx.h"
#include "RecoveryITEM.h"


RecoveryITEM::RecoveryITEM()
{
}


RecoveryITEM::~RecoveryITEM()
{
}

bool RecoveryITEM::Start()
{
	Item.Init(L"Assets/modelData/healitem.cmo");					//���f���̌Ăяo���B
	item_animClip[0].Load(L"Assets/animData/heal_rot.tka");	//�J���̃��[�h
	item_animClip[0].SetLoopFlag(true);						//���[�v�B
	m_anim.Init(
		Item,				//���f��
		item_animClip,		//�A�j���[�V�����N���b�v
		m_AnimClipNum		//�A�j���[�V�����̐�
	);
	m_anim.Play(0);			//�ŏ��̓���
	m_charaCon.Init(50.0f, 50.0f, m_position, enCollisionAttr_Enemy);	//�L�����R���̐ݒ�i���a�A�����A�����ʒu�B�j
	return true;
}

void RecoveryITEM::Update()
{
	Draw();
	Rotation();
	m_anim.Update(GameTime().GetFrameDeltaTime());
	Item.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	m_charaCon.SetPosition(m_position);
}

void RecoveryITEM::Death()
{
	g_goMgr.QutavaleyaAGO(this);
}

void RecoveryITEM::Draw()
{
	Item.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		1
	);
}

void RecoveryITEM::Rotation()
{
	//��ɉ�葱���܂��B
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY(), 2.0f);
	m_rotation.Multiply(qRot);
	Item.SetRotation(m_rotation);
}

void RecoveryITEM::HealAcquisition()
{
	m_timer += GameTime().GetFrameDeltaTime();
	if (m_timer > Limit)
	{
		Move = m_player->GetPosition();
		Move.Normalize();
	}
	m_moveSpeed += Move;
}