#include "stdafx.h"
#include "Title.h"
#include"Game.h"

Title::Title()
{
	m_sprite = g_goMgr.NewAGO<SpriteRender>();
	m_sprite->Init(L"Assets/sprite/title1.dds", 1280, 720);
	m_sprite->SetPivot(CVector2(0.5f,0.5f));
	Stage = First;
	//��
	m_soundEngine.Init();
	//BGM
	m_bgm[0].InitStreaming(L"Assets/sound/Result.wav");
	m_bgm[0].Play(true);
	m_bgm[0].SetVolume(0.15f);
	////���ʉ�
	m_bgm[1].Init(L"Assets/sound/Button.wav");
}

Title::~Title()
{
	g_goMgr.QutavaleyaAGO(m_sprite);
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
	Game* game = g_goMgr.NewAGO<Game>();
	game->SetStage(0);
	g_goMgr.QutavaleyaAGO(this);
}
