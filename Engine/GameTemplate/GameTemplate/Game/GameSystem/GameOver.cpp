#include "stdafx.h"
#include "GameOver.h"
#include"Game/Game.h"

GameOver::GameOver()
{
}


GameOver::~GameOver()
{
	g_goMgr.DeleteGO(m_sprite);
}

bool GameOver::Start()
{
	m_sprite = g_goMgr.NewGO<SpriteRender>();
	m_sprite->Init(L"Assets/sprite/GameOver.dds", SPRITERE_SOLUTION_W, SPRITERE_SOLUTION_H);
	return true;
}

void GameOver::Update()
{
	m_sprite->Update();
	Flag();
	if (g_pad[0].IsTrigger(enButtonA))
	{
		Death();
		this->SetUpdateFlag(false);
	}
}

void GameOver::Death()
{
	//ゲーム生成。タイトル削除。
	Game* game = g_goMgr.NewGO<Game>();
	game->SetStage(0);
	g_goMgr.DeleteGO(this);
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
	////場所と色の指定。
	//if (m_continueFlag) {
	//	m_font.DrawScreenPos(L"Retry", CVector2(0, 0),
	//		CVector4(0.0f, 0.0f, 0.0f, 1.0f));
	//}
	//else if (!m_continueFlag)
	//{
	//	m_font.DrawScreenPos(L"Exit", CVector2(0, 0),
	//		CVector4(0.0f, 0.0f, 0.0f, 1.0f)); 
	//}
	m_font.DrawScreenPos(L"Retry", CVector2(0, 0),
	CVector4(0.0f, 0.0f, 0.0f, 1.0f));
}