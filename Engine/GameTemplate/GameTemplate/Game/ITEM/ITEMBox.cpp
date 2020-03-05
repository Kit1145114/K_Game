#include "stdafx.h"
#include "ITEMBox.h"


ITEMBox::ITEMBox()
{
	Box.Init(L"Assets/modelData/BOX.cmo");			//���f���̌Ăяo���B
	box_animClip[open].Load(L"Assets/animData/box_open.tka");	//�J���̃��[�h
	box_animClip[open].SetLoopFlag(false);						//���J���B
	box_anim.Init(
		Box,				//���f��
		box_animClip,		//�A�j���[�V�����N���b�v
		m_AnimClipNum		//�A�j���[�V�����̐�
	);
	m_position = { 0.0f,-50.0f,0.0f };
	m_scale = {3.0f,3.0f,3.0f};
	m_charaCon.Init(100.0f, 50.0f, m_position);	//�L�����R���̐ݒ�i���a�A�����A�����ʒu�B�j
}

ITEMBox::~ITEMBox()
{
}
//�A�b�v�f�[�g�����B
void ITEMBox::Update()
{
	Draw();
	State();
	if (isOpen)
	{
		box_anim.Update(0.025f);	//�A�j���[�V�������t���[���P�ʂŕ`��B
	}
	Box.UpdateWorldMatrix(m_position, m_rotation, m_scale);
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

void ITEMBox::State()
{
	if (isOpen)
	{
		box_anim.Play(open);
	}
}