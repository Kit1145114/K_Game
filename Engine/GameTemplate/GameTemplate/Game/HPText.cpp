#include "stdafx.h"
#include "HPText.h"
#include"Player.h"

HPText::HPText()
{
	m_sprite = g_goMgr.NewAGO<SpriteRender>();
	m_sprite->Init(L"Assets/sprite/Midori.dds", 10, 40);
	m_sprite->SetPosition(m_position);
	m_sprite->SetPivot(PIVOT);
	m_scale = { 1.0,1.0f,0.0f };		//HP減ったり増えたりする用
}

HPText::~HPText()
{
	g_goMgr.QutavaleyaAGO(m_sprite);
}

void HPText::Update()
{
	HPUpdate();
}

void HPText::HPUpdate()
{
	m_scale = { m_HP, 1.0f,1.0f };
	m_sprite->SetScale(m_scale);
}