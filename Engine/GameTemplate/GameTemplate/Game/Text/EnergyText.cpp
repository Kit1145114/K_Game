#include "stdafx.h"
#include "EnergyText.h"
#include "Chara/Player.h"

EnergyText::EnergyText()
{
	m_sprite = g_goMgr.NewGO<SpriteRender>();
	m_sprite->Init(L"Assets/sprite/Energy.dds", 1.666, 30);
	m_sprite->SetPosition(m_position);
	m_sprite->SetPivot(PIVOT);
}


EnergyText::~EnergyText()
{
	g_goMgr.DeleteGO(m_sprite);
}

void EnergyText::EnergyUpdate()
{
	m_scale = { m_energy, 1.0f,1.0f };
	m_sprite->SetScale(m_scale);
}

void EnergyText::Update()
{
	EnergyUpdate();
}