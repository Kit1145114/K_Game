#include "stdafx.h"
#include "GameOver.h"


GameOver::GameOver()
{
	m_sprite = g_goMgr.NewAGO<SpriteRender>();
	m_sprite->Init(L"Assets/sprite/GameOver.dds", 1280, 720);
	m_sprite->SetPivot(CVector2(0.5f, 0.5f));
}


GameOver::~GameOver()
{
	g_goMgr.QutavaleyaAGO(m_sprite);
}

bool GameOver::Start()
{
	//m_sprite = g_goMgr.NewAGO<SpriteRender>();
	//m_sprite->Init(L"Assets/sprite/GameOver.dds", 1280, 720);
	//m_sprite->SetPivot(CVector2(0.5f, 0.5f));
	return true;
}

void GameOver::Update()
{
	Flag();
	m_sprite->Update();
}

void GameOver::Death()
{

}

void GameOver::Flag()
{
	if (m_continueFlag && g_pad[0].IsTrigger(enButtonLeft))
	{
		m_continueFlag = false;
	}
	else if (!m_continueFlag && g_pad[0].IsTrigger(enButtonRight))
	{
		m_continueFlag = true;
	}
}

void GameOver::FontRender()
{
	//èÍèäÇ∆êFÇÃéwíËÅB
	if (m_continueFlag) {
		m_font.DrawScreenPos(L"Retry", CVector2(250, -150),
			CVector4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	else if (!m_continueFlag)
	{
		m_font.DrawScreenPos(L"Exit", CVector2(-250, -150),
			CVector4(0.0f, 0.0f, 0.0f, 1.0f)); 
	}
		//wchar_t text[10];
		//swprintf_s(text, L"%d", HP);
		//m_font.DrawScreenPos(text, CVector2(400, 40),
		//	CVector4(0.0f, 1.0f, 0.0f, 1.0f));
}