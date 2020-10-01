#include "stdafx.h"
#include "MAP.h"
#include"../Chara/Enemys.h"
#include"../Chara/Golem.h"
#include"../Chara/Titan.h"
#include"../Chara/Player.h"

MAP::MAP()
{
}


MAP::~MAP()
{
}

bool MAP::Start()
{
	switch (m_stage)
	{
		case 0:
		Map.Init(L"Assets/modelData/FirstStage.cmo");
		break;
		case 1:
		Map.Init(L"Assets/modelData/MAP.cmo");	//�}�b�v�̕`��
		break;
	}
	m_physicsStaticObj.CreateMeshObject(Map, m_position, CQuaternion::Identity());
	return true;
}

void MAP::Update()
{
	//�}�b�v�̃A�b�v�f�[�g�����ɋL�q�B
	Map.UpdateWorldMatrix(m_position, CQuaternion::Identity(), m_scale);
}
void MAP::Draw()
{
	Map.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		0
	);
}