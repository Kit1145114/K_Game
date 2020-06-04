#include "stdafx.h"
#include "Door.h"
#include"Chara/Player.h"
#include"GameSystem/Title.h"

Door::Door()
{
	Gate.Init(L"Assets/modelData/Door.cmo");						//モデルの呼び出し。
	//m_position = {0.0f,-300.0f,1000.0f};							//デバック用のポジションだったもの
}

Door::~Door()
{
	g_goMgr.QutavaleyaAGO(m_player);
	g_goMgr.QutavaleyaAGO(this);
}

void Door::Update()
{
	Draw();
	PlayerDistance();
	Gate.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}

void Door::Draw()
{
	Gate.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		1
	);
}

void Door::PlayerDistance()
{
	m_distance = m_position - m_player->GetPosition();
	if (m_distance.Length() < m_warpDistance && g_pad->IsTrigger(enButtonB))
	{
		ChangeStage = true;
	}
}