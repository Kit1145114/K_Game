#include "stdafx.h"
#include "ChangeScreen.h"
#include"GameConst.h"
#include"Game/Game.h"

ChangeScreen::ChangeScreen()
{
	m_sprite = g_goMgr.NewAGO<SpriteRender>();
	m_sprite->Init(L"Assets/sprite/nowroading.dds", 1280, 720);
	m_sprite->SetPivot(CVector2(0.5f, 0.5f));
}


ChangeScreen::~ChangeScreen()
{
	g_goMgr.QutavaleyaAGO(m_sprite);
}

void ChangeScreen::Update()
{
	m_sprite->Update();
	m_time += GameTime().GetFrameDeltaTime();
	if (m_time >= m_changeTime)
	{
		Death();
		this->SetUpdateFlag(false);
	}
}

void ChangeScreen::Death()
{
	//ゲーム生成。タイトル削除。
	game = g_goMgr.NewAGO<Game>();
	game->SetStage(1);
	game->SetPlayerHp(m_playerHp);
	m_time = ZERO;
	g_goMgr.QutavaleyaAGO(this);
}