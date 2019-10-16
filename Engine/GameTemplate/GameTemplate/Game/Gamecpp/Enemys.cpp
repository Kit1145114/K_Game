#include "stdafx.h"
#include "Enemys.h"
#include"StoneEnemy.h"

Enemys::Enemys()
{
	/*switch (m_ID)
	{
	case 0:
		model.Init(L"Assets/modelData/Enemy3.cmo");
		break;
	}*/
}

Enemys::~Enemys()
{
}

void Enemys::Init(float HP, float Attack, float Defense, float Speed, SkinModel model)
{
	m_HP		=	HP;				//ヒットポイントを代入
	m_ATK		=	Attack;			//攻撃力を代入。
	m_DEF		=	Defense;		//防御力を代入。
	m_SPD		=	Speed;			//基礎速度を代入。
	m_sm		=	model;			//エネミーのモデル。
}

/*void Enemys::Update()
{
	Draw();
}
void Enemys::Draw()
{
	m_sm.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}*/