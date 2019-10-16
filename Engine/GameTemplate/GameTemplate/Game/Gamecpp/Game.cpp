#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include"MAP.h"
#include"GameCamera.h"
#include"GameObjectManajer.h"
#include"Enemys.h"
#include"StoneEnemy.h"
#include "Titan.h"
#include "Golem.h"

Game* Game::m_instance = nullptr;	//�Q�[���̃C���X�^���X�̐���

Game::Game()
{
	//�����A�Q�[�������ɑ��݂��Ă�����
	if (m_instance != nullptr)
	{
		//�Q�[�����I��������B
		std::abort();
	}
	else {
		//�����Ȃ��ꍇ�̓Q�[�������B
		m_instance = this;
	}
	//�Q�[���̃X�^�[�g�֐��Ăяo���B
	Start();			
}


Game::~Game()
{
	//�Q�[���̍폜�ƂƂ��ɁA�C���X�^���X��null������B
	m_instance = nullptr;	
	//�����v���C���[�������ĂȂ�������
	if (player != nullptr)
	{
		//�v���C���[���폜�B
		g_goMgr.QutavaleyaAGO(player);
	}
	//�����A�}�b�v�������ĂȂ�������
	if (map != nullptr)
	{
		//�}�b�v���폜�B
		g_goMgr.QutavaleyaAGO(map);
	}
	for (int i = 0; i < enemyNum; i++)
	{
		if (enemys[i] != nullptr)
		{
			g_goMgr.QutavaleyaAGO(enemys[i]);
		}
	}
}

Game* Game::GetInstance()
{
	//�ǂ�����ł��Q�[�����Ăяo����悤��
	//�C���X�^���X��return�ŕԂ��B
	return m_instance;
}

//NewGO������̂𕪂��ē���Ă���B
bool Game::Start()
{
	//�v���C���[��NewGO�Ő����B
	player = g_goMgr.NewAGO<Player>();
	//�}�b�v��NewGO�Ő����B
	map = g_goMgr.NewAGO<MAP>();
	//�Q�[���J������NewGO�Ő����B
	g_Camera = g_goMgr.NewAGO<GameCamera>();
	g_Camera->SetPlayer(player);
	//�G�l�~�[�𐶐��B
	enemys[0] = g_goMgr.NewAGO<StoneEnemy>();
	enemys[1] = g_goMgr.NewAGO<Golem>();
	enemys[2] = g_goMgr.NewAGO<Titan>();
	return true;
}

void Game::Update()
{

}