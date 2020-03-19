#include "stdafx.h"
#include "GameClear.h"
#include"../Title.h"

GameClear::GameClear()
{
	m_sprite = g_goMgr.NewAGO<SpriteRender>();
	m_sprite->Init(L"Assets/sprite/GameClear.dds", 1280, 720);
	m_sprite->SetPivot(CVector2(0.5f, 0.5f));
	//��
	m_soundEngine.Init();
	//BGM
	m_bgm[0].Init(L"Assets/sound/Result.wav");
	m_bgm[0].Play(true);
	m_bgm[0].SetVolume(0.15f);
	//���ʉ�
	m_bgm[1].Init(L"Assets/sound/Button.wav");
	m_bgm[1].SetVolume(0.4f);
}


GameClear::~GameClear()
{
	g_goMgr.QutavaleyaAGO(m_sprite);
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
	//�Q�[�������B�^�C�g���폜�B
	Title* title = g_goMgr.NewAGO<Title>();
	g_goMgr.QutavaleyaAGO(this);
}