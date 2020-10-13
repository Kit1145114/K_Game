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
		map.Init(L"Assets/modelData/FirstStage.cmo");
		break;
		case 1:
		map.Init(L"Assets/modelData/MAP.cmo");	//マップの描画
		break;
	}
	m_physicsStaticObj.CreateMeshObject(map, m_position, CQuaternion::Identity());
	return true;
}

void MAP::Update()
{
	//マップのアップデートを下に記述。
	map.UpdateWorldMatrix(m_position, CQuaternion::Identity(), m_scale);
}
void MAP::Draw()
{
	map.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		0
	);
}