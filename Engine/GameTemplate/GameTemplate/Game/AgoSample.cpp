#include "stdafx.h"
#include "AgoSample.h"


AgoSample::AgoSample()
{
	Ago.Init(L"Assets/modelData/AGO.cmo");	//�}�b�v�̕`��
	//m_PSO.CreateMeshObject(Ago,, CQuaternion::Identity());
	m_PhyGhostObj.CreateBox(m_position, CQuaternion::Identity(),m_scale);
}


AgoSample::~AgoSample()
{

}

void AgoSample::Update()
{
	//�}�b�v�̃A�b�v�f�[�g�����ɋL�q�B
	Ago.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
	//Draw�֐��̌Ăяo��
	Draw();
}
void AgoSample::Draw()
{
	Ago.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}