#include "stdafx.h"
#include "Title.h"
#include"Game.h"

Title::Title()
{
	m_sprite = g_goMgr.NewAGO<SpriteRender>();
	m_sprite->Init(L"Assets/sprite/title1.dds", 1280, 720);
	m_sprite->SetPivot(CVector2(0.5f,0.5f));
}

Title::~Title()
{
	g_goMgr.QutavaleyaAGO(m_sprite);
}
//タイトル
void Title::Update()
{
	m_sprite->Update();
	if (g_pad[0].IsTrigger(enButtonA))
	{
		Death();
		this->SetUpdateFlag(false);
	}
}
//タイトル削除。
void Title::Death()
{
	//ゲーム生成。タイトル削除。
	game = g_goMgr.NewAGO<Game>();
	g_goMgr.QutavaleyaAGO(this);
}
