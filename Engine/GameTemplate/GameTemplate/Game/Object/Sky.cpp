#include "stdafx.h"
#include "Sky.h"


Sky::Sky()
{
}


Sky::~Sky()
{
}

bool Sky::Start()
{
	SkyModel.Init(L"Assets/modelData/Sky.cmo");	//��̕`��
	return true;
}
void Sky::Update()
{
	//��̃A�b�v�f�[�g�����ɋL�q�B
	SkyModel.UpdateWorldMatrix(m_position, CQuaternion::Identity(), m_scale);
}

void Sky::Draw()
{
	//�h���[
	SkyModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		0
	);
}