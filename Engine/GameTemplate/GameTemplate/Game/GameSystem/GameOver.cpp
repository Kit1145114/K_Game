#include "stdafx.h"
#include "GameOver.h"
#include"Game.h"

GameOver::GameOver()
{
}


GameOver::~GameOver()
{
	g_goMgr.QutavaleyaAGO(m_sprite);
}

bool GameOver::Start()
{
	m_sprite = g_goMgr.NewAGO<SpriteRender>();
	m_sprite->Init(L"Assets/sprite/GameOver.dds", 1280, 720);
	m_sprite->SetPivot(CVector2(0.5f, 0.5f));
	return true;
}

void GameOver::Update()
{
	m_sprite->Update();
	//if (g_pad[0].IsTrigger(enButtonA))
	//{
	//	Death();
	//	this->SetUpdateFlag(false);
	//}
}

void GameOver::Death()
{
	//ゲーム生成。タイトル削除。
	Game* game = g_goMgr.NewAGO<Game>();
	game->SetStage(0);
	g_goMgr.QutavaleyaAGO(this);
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
	//場所と色の指定。
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