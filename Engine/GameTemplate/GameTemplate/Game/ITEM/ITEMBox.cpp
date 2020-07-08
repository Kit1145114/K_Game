#include "stdafx.h"
#include "ITEMBox.h"


ITEMBox::ITEMBox()
{
	
}

ITEMBox::~ITEMBox()
{
}

bool ITEMBox::Start()
{
	Box.Init(L"Assets/modelData/BOX.cmo");						//���f���̌Ăяo���B
	box_animClip[open].Load(L"Assets/animData/box_open.tka");	//�J���̃��[�h
	box_animClip[open].SetLoopFlag(false);						//���J���B
	box_animClip[close].Load(L"Assets/animData/box_idle.tka");	//���Ă郍�[�h
	box_animClip[close].SetLoopFlag(false);						//���J���B
	box_anim.Init(
		Box,				//���f��
		box_animClip,		//�A�j���[�V�����N���b�v
		m_AnimClipNum		//�A�j���[�V�����̐�
	);
	box_anim.Play(close);
	state = close;
	m_charaCon.Init(100.0f, 50.0f, m_position, enCollisionAttr_Enemy);	//�L�����R���̐ݒ�i���a�A�����A�����ʒu�B�j
	m_effect[0] = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/drop.efk");
	return true;
}
//�A�b�v�f�[�g�����B
void ITEMBox::Update()
{
	//Draw();
	State();
	box_anim.Update(GameTime().GetFrameDeltaTime());
	Box.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());
}
//�h���[�֐��B
void ITEMBox::Draw()
{
	Box.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		1
	);
}
//�X�e�[�g
void ITEMBox::State()
{
	switch (state)
	{
	case ITEMBox::open:
		box_anim.Play(open);
		Open();
		break;
	case ITEMBox::close:
		Close();
		box_anim.Play(close);
		break;
	case ITEMBox::death:
		Delete();
		break;
	}
}
//�폜
void ITEMBox::Delete()
{
	g_goMgr.QutavaleyaAGO(this);
}
//�J���邩�ǂ����B
void ITEMBox::Open()
{

	if (!box_anim.IsPlaying())
	{
		//��
		if (!itemDrop_flag)
		{
			RItem = g_goMgr.NewAGO<RecoveryITEM>();
			RItem->SetPositon(m_position);
			RItem->SetPlayer(m_player);
			itemDrop_flag = true;
		}
		m_timer += GameTime().GetFrameDeltaTime();
		if (m_timer > m_deathTime) {
			state = death;
		}
	}
}

void ITEMBox::Close()
{
	if (isOpen)
	{
		state = open;
		m_playEffectHandle = g_effektEngine->Play(m_effect[0]);
		g_effektEngine->SetPosition(m_playEffectHandle, m_position);
	}
}