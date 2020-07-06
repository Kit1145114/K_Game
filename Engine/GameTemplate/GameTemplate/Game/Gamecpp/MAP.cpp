#include "stdafx.h"
#include "MAP.h"
#include"../Chara/Enemys.h"
#include"../Chara/Golem.h"
#include"../Chara/StoneEnemy.h"
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
		Map.Init(L"Assets/modelData/FirstMap.cmo");
		break;
		case 1:
		Map.Init(L"Assets/modelData/MAP.cmo");	//マップの描画
		break;
	}
	//Map.Init(L"Assets/modelData/MAP.cmo");	//マップの描画
	//Map.Init(L"Assets/modelData/FirstMap.cmo");
	m_physicsStaticObj.CreateMeshObject(Map, m_position, CQuaternion::Identity());
	//Map.SetShadowCaster(false);
	return true;
}

void MAP::Update()
{
	//マップのアップデートを下に記述。
	Map.UpdateWorldMatrix(m_position, CQuaternion::Identity(), m_scale);
	//Draw関数の呼び出し
	//Draw();
}
void MAP::Draw()
{
	Map.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		0
	);
}