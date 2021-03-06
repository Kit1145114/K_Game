#include "stdafx.h"
#include "Title.h"
#include"Game/Game.h"

Title::Title()
{
}
Title::~Title()
{
	g_goMgr.DeleteGO(m_sprite);
}
bool Title::Start()
{
	m_sprite = g_goMgr.NewGO<SpriteRender>();
	m_sprite->Init(L"Resource/sprite/title.dds", SPRITERE_SOLUTION_W, SPRITERE_SOLUTION_H);
	m_Fade = g_goMgr.NewGO<Fade>();
	m_Fade->StartFadeIn();
	Stage = StageNum::First;
	//音
	m_soundEngine.Init();
	//BGM
	m_bgm[0].InitStreaming(L"Assets/sound/Result.wav");
	m_bgm[0].Play(true);
	m_bgm[0].SetVolume(m_volume);
	////効果音
	m_bgm[1].Init(L"Assets/sound/Button.wav");
	return true;
}
//タイトル
void Title::Update()
{
	m_sprite->Update();
	if (g_pad[0].IsTrigger(enButtonA))
	{
		m_Fade->StartFadeOut();
		m_bgm[1].Play(true);
		Death();
		this->SetUpdateFlag(false);
	}
}
//タイトル削除。
void Title::Death()
{
	//ゲーム生成。タイトル削除。
	//ゲームだよ！！
	Game* game = g_goMgr.NewGO<Game>();
	game->SetStage(m_stageNum);
	g_goMgr.DeleteGO(this);
}
