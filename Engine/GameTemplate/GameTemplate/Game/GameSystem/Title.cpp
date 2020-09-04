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
	m_sprite->Init(L"Resource/sprite/title.dds", 1280, 720);
	m_sprite->SetPivot(CVector2(0.5f, 0.5f));
	Stage = First;
	//��
	m_soundEngine.Init();
	//BGM
	m_bgm[0].InitStreaming(L"Assets/sound/Result.wav");
	m_bgm[0].Play(true);
	m_bgm[0].SetVolume(0.30f);
	////���ʉ�
	m_bgm[1].Init(L"Assets/sound/Button.wav");
	return true;
}
//�^�C�g��
void Title::Update()
{

	m_sprite->Update();
	if (g_pad[0].IsTrigger(enButtonA))
	{
		m_bgm[1].Play(true);
		Death();
		this->SetUpdateFlag(false);
	}
}
//�^�C�g���폜�B
void Title::Death()
{
	//�Q�[�������B�^�C�g���폜�B
	//�Q�[������I�I
	Game* game = g_goMgr.NewGO<Game>();
	game->SetStage(m_stageNum);
	g_goMgr.DeleteGO(this);
}
