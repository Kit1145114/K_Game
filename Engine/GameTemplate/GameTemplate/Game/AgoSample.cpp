#include "stdafx.h"
#include "AgoSample.h"


AgoSample::AgoSample()
{
	Ago.Init(L"Assets/modelData/AGO.cmo");	//マップの描画
	//m_PSO.CreateMeshObject(Ago,, CQuaternion::Identity());
	m_PhyGhostObj.CreateBox(m_position, CQuaternion::Identity(),m_scale);
}


AgoSample::~AgoSample()
{

}

void AgoSample::Update()
{
	//マップのアップデートを下に記述。
	Ago.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
	//Draw関数の呼び出し
	Draw();
}
void AgoSample::Draw()
{
	Ago.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}