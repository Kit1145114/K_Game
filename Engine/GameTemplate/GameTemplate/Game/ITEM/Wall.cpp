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
	m_wall.Init(L"Assets/modelData/Wall.cmo");						//ÉÇÉfÉãÇÃåƒÇ—èoÇµÅB
	m_physicsStaticObj.CreateMeshObject(m_wall, m_position, m_rotation);
	//m_charaCon.Init(50.0f, 150.0f, m_position);		//îªíËÇÃëÂÇ´Ç≥
	return true;
}

void Wall::Update()
{
	Draw();
	m_wall.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	//m_charaCon.SetPosition(m_position);
}

void Wall::Draw()
{
	m_wall.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		1
	);
}