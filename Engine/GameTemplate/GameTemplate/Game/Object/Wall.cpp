#include "stdafx.h"
#include "Wall.h"

Wall::Wall()
{

}

Wall::~Wall()
{
	m_charaCon.RemoveRigidBoby();
}

bool Wall::Start()
{
	m_wall.Init(L"Assets/modelData/Wall.cmo");							//���f���̌Ăяo���B
	m_se.Init(L"Assets/sound/gogogogo1.wav");							//���ʉ��̌Ăяo���B
	m_physicsStaticObj.CreateMeshObject(m_wall, m_position, m_rotation);
	wallState = w_idle;													//
	w_animClip[w_idle].Load(L"Assets/animData/Wall_idle.tka");
	w_animClip[w_idle].SetLoopFlag(true);
	w_animClip[w_move].Load(L"Assets/animData/Wall_Move.tka");
	w_animClip[w_move].SetLoopFlag(true);
	w_anim.Init(
		m_wall,				//�A�j���[�V�����𗬂��X�L�����f���B
							//����ŃA�j���[�V�����ƃX�L�����f�����֘A�t������B
		w_animClip,			//�A�j���[�V�����N���b�v�̔z��B
		m_AnimClipNum		//�A�j���[�V�����N���b�v�̐�
	);
	return true;
}
//�X�V
void Wall::Update()
{
	Draw();
	State();
	//�A�j���[�V�����̍X�V
	w_anim.Update(GameTime().GetFrameDeltaTime());
	//���f���̍X�V
	m_wall.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}
//�h���[
void Wall::Draw()
{
	m_wall.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		1
	);
}
//���
void Wall::State()
{
	switch (wallState)
	{
	case Wall::w_idle:
		Idle();				//�����ĂȂ�����
		break;
	case Wall::w_move:
		Move();				//�����Ƃ��̏���
		break;
	case Wall::w_death:
		Death();			//���ʂƂ��B
		break;
	}
}
//��
void Wall::Death()
{
	m_physicsStaticObj.RemoveRigidBoby();
	g_goMgr.DeleteGO(this);
}
//��������
void Wall::Move()
{
	//�A�j���[�V�����Đ��B
	//w_anim.Play(w_move,0.1f);
	m_se.Play(false);
	m_position.y -= 20.0f;
	//�����Ń^�C�}�[���񂵂܂��B
	m_timer += GameTime().GetFrameDeltaTime();
	if (m_timer > 1.5f) {
		wallState = w_death;
	}
}
//�����Ȃ��Ƃ�
void Wall::Idle()
{
	w_anim.Play(w_idle, 0.1f);
	if (m_move_flag)
	{
		wallState = w_move;
	}
}