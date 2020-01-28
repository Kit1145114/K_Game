#include "stdafx.h"
#include "MAP.h"
#include"Enemys.h"
#include"Golem.h"
#include"StoneEnemy.h"
#include"Titan.h"
#include"Player.h"

MAP::MAP()
{
	Map.Init(L"Assets/modelData/MAP.cmo");	//�}�b�v�̕`��
	m_physicsStaticObj.CreateMeshObject(Map, position, CQuaternion::Identity());
}


MAP::~MAP()
{
}

void MAP::Update()
{
	//�}�b�v�̃A�b�v�f�[�g�����ɋL�q�B
	Map.UpdateWorldMatrix(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	//Draw�֐��̌Ăяo��
	Draw();
}
void MAP::Draw()
{
	Map.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		0
	);
}