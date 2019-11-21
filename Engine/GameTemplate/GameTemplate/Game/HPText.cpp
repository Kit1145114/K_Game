#include "stdafx.h"
#include "HPText.h"


HPText::HPText()
{
	m_sprite = g_goMgr.NewAGO<SpriteRender>();
	m_sprite->Init(L"Assets/sprite/Midori.dds", wide, 50);
	m_sprite->SetPosition(m_position);
}

HPText::~HPText()
{
	g_goMgr.QutavaleyaAGO(m_sprite);

}

void HPText::Update()
{
	int min = 0;
	int max = 500;
	if (wide <= max)
	{
		wide++;
	}
	else if (wide >= min)
	{
		wide--;
	}
	;
}