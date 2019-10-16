#include "stdafx.h"
#include "GameCamera.h"
#include"Player.h"

GameCamera::GameCamera()
{
	//�J�������������B
	g_camera3D.SetPosition({ 0.0f, 100.0f, 300.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(10000.0f);
}


GameCamera::~GameCamera()
{
}

void GameCamera::Update()
{
	CVector3 targetPos = m_player->GetPosition();	//�v���C���[�̃|�W�V�������擾�B
	targetPos.y += 100.0f;							//�ォ�猩�����̂�100.0f����B
	CVector3 position = targetPos;					//�|�W�V�����Ƀv���C���[�̃|�W�V����������B
	position.z -= 400.0f;							//��납�猩���̂�400.0f����
	g_camera3D.SetTarget(targetPos);				//�v���C���[�̃|�W�V�������^�[�Q�b�g�ɁB
	g_camera3D.SetPosition(position);				//�J�����̃|�W�V������position��
	g_camera3D.Update();							//�A�b�v�f�[�g�B
}