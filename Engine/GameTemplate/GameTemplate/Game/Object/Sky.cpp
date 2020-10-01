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
	SkyModel.Init(L"Assets/modelData/Sky.cmo");	//空の描画
	return true;
}
void Sky::Update()
{
	//空のアップデートを下に記述。
	SkyModel.UpdateWorldMatrix(m_position, CQuaternion::Identity(), m_scale);
}

void Sky::Draw()
{
	//ドロー
	SkyModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		0
	);
}