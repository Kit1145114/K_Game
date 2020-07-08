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
	m_wall.Init(L"Assets/modelData/Wall.cmo");							//���f���̌Ăяo���B
	m_physicsStaticObj.CreateMeshObject(m_wall, m_position, m_rotation);
	wallState = w_idle;													//
	return true;
}
//�X�V
void Wall::Update()
{
	Draw();
	State();
	m_wall.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}
//�h���[
void Wall::Draw()
{
	m_wall.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		1
	);
}
//���
void Wall::State()
{
	switch (wallState)
	{
	case Wall::w_idle:

		break;
	case Wall::w_move:

		break;
	case Wall::m_death:
		break;
	}
}
//��
void Wall::Death()
{

}