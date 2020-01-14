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
#include"Sample.h"
#include"HPText.h"
#include"Enemys.h"
#include"AgoSample.h"

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
	//�����G�l�~�[�������ĂȂ�������B
		if (enemys != nullptr)
	{
		//�G�l�~�[���폜�B
		g_goMgr.QutavaleyaAGO(enemys);
	}

	//for (int i = 0; i < ENEMY_NUM; i++)
	//{
	//	if (enemys != nullptr)
	//	{
	//		//�G�l�~�[���폜�B
	//		g_goMgr.QutavaleyaAGO(enemys[i]);
	//	}
	//}
}

Game* Game::GetInstance()
{
	//�ǂ�����ł��Q�[�����Ăяo����悤��
	//�C���X�^���X��return�ŕԂ��B
	return m_instance;
}
//�Q�[���J�n�B�B�B�B�I�I�I�I�B
bool Game::Start()
{
	////�v���C���[��NewGO�Ő����B
	//player = g_goMgr.NewAGO<Player>();
	////�}�b�v��NewGO�Ő����B
	//map = g_goMgr.NewAGO<MAP>();
	////�Q�[���J������NewGO�Ő����B
	//g_Camera = g_goMgr.NewAGO<GameCamera>();
	//g_Camera->SetPlayer(player);
	//g_Camera->SetEnemysList(m_goList);
	////�G�l�~�[�𐶐��B
	//enemys[0] = g_goMgr.NewAGO<StoneEnemy>();
	//m_goList.push_back(enemys[0]);
	//enemys[0]->SetPlayer(player);
	//enemys[1] = g_goMgr.NewAGO<Golem>();
	//m_goList.push_back(enemys[1]);
	//enemys[1]->SetPlayer(player);
	//enemys[2] = g_goMgr.NewAGO<Titan>();
	//m_goList.push_back(enemys[2]);
	//enemys[2]->SetPlayer(player);
	//hp_bar = g_goMgr.NewAGO<HPText>();
	//player->SetEnemysList(m_goList);
	//player->SetPosition(m_initPlayerPos);
	mapLevel.Init(L"Assets/level/FirstStage.tkl",
		[&](LevelObjectData& objData)
	{
		if (objData.EqualObjectName(L"Enemy2") == true) {
			//�G(��l��)�̃I�u�W�F�N�g�B
			Enemys* enemys = g_goMgr.NewAGO<Golem>();
			enemys->SetPosition(objData.position);
			enemys->SetRotation(objData.rotation);
			//enemys->SetScale(objData.scale);
			//��ō폜����̂Ń��X�g�ɐς�ŋL�����Ă����B
			m_enemysList.push_back(enemys);
			//�t�b�N�����̂�true��Ԃ��B
			return true;
		}
		else if (objData.EqualObjectName(L"Enemy3") == true) {
			//�G(��l��)�̃I�u�W�F�N�g�B
			Enemys* enemys = g_goMgr.NewAGO<StoneEnemy>();
			enemys->SetPosition(objData.position);
			enemys->SetRotation(objData.rotation);
			//enemys->SetScale(objData.scale);
			//��ō폜����̂Ń��X�g�ɐς�ŋL�����Ă����B
			m_enemysList.push_back(enemys);
			//�t�b�N�����̂�true��Ԃ��B
			return true;
		}
		else if (objData.EqualObjectName(L"RobbotEnemy1") == true) {
			//�G(��l��)�̃I�u�W�F�N�g�B
			enemys = g_goMgr.NewAGO<Titan>();
			enemys->SetPosition(objData.position);
			enemys->SetRotation(objData.rotation);
			//enemys->SetScale(objData.scale);
			//��ō폜����̂Ń��X�g�ɐς�ŋL�����Ă����B
			m_enemysList.push_back(enemys);
			//�t�b�N�����̂�true��Ԃ��B
			return true;
		}
		else if (objData.EqualObjectName(L"Player") == true) {
			player = g_goMgr.NewAGO<Player>();
			player->SetPosition(objData.position);
			player->SetRotation(objData.rotation);
			player->SetEnemysList(m_enemysList);
			//�t�b�N�����̂�true��Ԃ��B
			return true;
		}
		else if (objData.EqualObjectName(L"MAP") == true) {
			map = g_goMgr.NewAGO<MAP>();
			map->SetPosition(objData.position);
			//�t�b�N�����̂�true��Ԃ��B
			return true;
		}
		return true;
	});
	for (auto enemy : m_enemysList) {
		enemy->SetPlayer(player);
	}
	player->SetEnemysList(m_enemysList);
	g_Camera = g_goMgr.NewAGO<GameCamera>();
	g_Camera->SetPlayer(player);
	hp_bar = g_goMgr.NewAGO<HPText>();
	hp_bar->SetPlayerHP(player->GetPlayerHP());
	return true;
}
//�Q�[���̃A�b�v�f�[�g�B
void Game::Update()
{
	hp_bar->SetPlayerHP(player->GetPlayerHP());
	//Render();
}

