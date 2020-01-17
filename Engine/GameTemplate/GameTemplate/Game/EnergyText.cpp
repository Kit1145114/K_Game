#include "stdafx.h"
#include "EnergyText.h"
#include "Player.h"

EnergyText::EnergyText()
{
	m_sprite = g_goMgr.NewAGO<SpriteRender>();
	m_sprite->Init(L"Assets/sprite/glass.dds", 3.35, 45);
	m_sprite->SetPosition(m_position);
}


EnergyText::~EnergyText()
{
	g_goMgr.QutavaleyaAGO(m_sprite);
}

void EnergyText::EnergyUpdate()
{
	m_scale = { m_Energy, 1.0f,1.0f };
	m_sprite->SetPivot(pivot);
	m_sprite->SetScale(m_scale);
}

void EnergyText::Update()
{
	EnergyUpdate();
}