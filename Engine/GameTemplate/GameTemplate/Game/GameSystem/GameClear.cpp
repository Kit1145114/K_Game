#include "stdafx.h"
#include "GameClear.h"
#include"../GameSystem/Title.h"

GameClear::GameClear()
{
	m_sprite = g_goMgr.NewGO<SpriteRender>();
	m_sprite->Init(L"Assets/sprite/GameClear.dds", SPRITERE_SOLUTION_W, SPRITERE_SOLUTION_H);
	//音
	m_soundEngine.Init();
	//BGM
	m_bgm[0].Init(L"Assets/sound/Result.wav");
	m_bgm[0].Play(true);
	m_bgm[0].SetVolume(m_volume);
	//効果音
	m_bgm[1].Init(L"Assets/sound/Button.wav");
}


GameClear::~GameClear()
{
	g_goMgr.DeleteGO(m_sprite);
}

void GameClear::Update()
{
	m_sprite->Update();
	if (g_pad[0].IsTrigger(enButtonA))
	{
		m_bgm[1].Play(true);
		Death();
		this->SetUpdateFlag(false);
	}
}

void GameClear::Death()
{
	//ゲーム生成。タイトル削除。
	Title* title = g_goMgr.NewGO<Title>();
	g_goMgr.DeleteGO(this);
}