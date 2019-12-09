#include "stdafx.h"
#include "HPText.h"


HPText::HPText()
{
	m_sprite = g_goMgr.NewAGO<SpriteRender>();
	m_sprite->Init(L"Assets/sprite/Midori.dds", 10, 40);
	m_sprite->SetPosition(m_position);
	m_scale = { wide,1.0f,0.0f };		//HPå∏Ç¡ÇΩÇËëùÇ¶ÇΩÇËÇ∑ÇÈóp
}

HPText::~HPText()
{
	g_goMgr.QutavaleyaAGO(m_sprite);
}

void HPText::Update()
{
	int min = 0;
	int max = 50;
	//if (time <= max)
	//{
	//	time++;
	//	wide += 1.0f;
	//}
	//if (time >= min)
	//{
	//	time--;
	//	wide -= 1.0f;
	//}
	//m_sprite->AddScaleX(wide);
	//m_sprite->SetScale();
}