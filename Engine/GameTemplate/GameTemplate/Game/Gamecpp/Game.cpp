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
#include"Boss.h"
#include"Sample.h"
#include"HPText.h"
#include"EnergyText.h"
#include"Enemys.h"
#include"Title.h"
#include"ITEM/ITEMBox.h"

Game* Game::m_instance = nullptr;	//�Q�[���̃C���X�^���X�̐���

Game::Game()
{
	m_soundEngine.Init();
	m_bgm.Init(L"Assets/sound/Result.wav");
	m_bgm.Play(true);
	m_bgm.SetVolume(0.15f);
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
	////�Q�[���̃X�^�[�g�֐��Ăяo���B
	//switch (m_stage)
	//{
	//case FirstStage:
	//	Start();
	//	break;
	//case SecondStage:
	//	NewBoss();
	//	break;
	//}
	//Start();
	NewBoss();
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
	for (auto enemy : m_enemysList) {
		if (enemy != nullptr) {
			g_goMgr.QutavaleyaAGO(enemy);
		}
	}
	//�J�����폜
	if (g_Camera != nullptr)
	{
		g_goMgr.QutavaleyaAGO(g_Camera);
	}
	//HP�o�[�폜
	if (hp_bar != nullptr) {
		g_goMgr.QutavaleyaAGO(hp_bar);
	}
	//�G�i�W�[�o�[�폜
	if (energy_bar != nullptr) {
		g_goMgr.QutavaleyaAGO(energy_bar);
	}
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
			//player->SetEnemysList(m_enemysList);
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
	energy_bar = g_goMgr.NewAGO<EnergyText>();
	energy_bar->SetPlayerEnergy(player->GetPlayerEnergy());
	return true;
}
//�Q�[���̃A�b�v�f�[�g�B
void Game::Update()
{
	m_soundEngine.Update();
	hp_bar->SetPlayerHP(player->GetPlayerHP());
	energy_bar->SetPlayerEnergy(player->GetPlayerEnergy());
	bool isLive = false;
	for (auto enemy : m_enemysList) {
		if (!enemy->GetIsDead())
		{
			isLive = true;
		}
		/*else
		{
			isLive = false;
		}*/
	}
	if (!isLive)
	{
		Title* title = g_goMgr.NewAGO<Title>();
		g_goMgr.QutavaleyaAGO(this);
	}
}
//�{�X�o���p
bool Game::NewBoss()
{
	mapLevel.Init(L"Assets/level/BossStage.tkl",
		[&](LevelObjectData& objData)
	{
		if (objData.EqualObjectName(L"RobbotBoss") == true) {
			//�G(��l��)�̃I�u�W�F�N�g�B
			Enemys* enemys = g_goMgr.NewAGO<Boss>();
			enemys->SetPosition(objData.position);
			enemys->SetRotation(objData.rotation);
			//��ō폜����̂Ń��X�g�ɐς�ŋL�����Ă����B
			m_enemysList.push_back(enemys);
			//�t�b�N�����̂�true��Ԃ��B
			return true;
		}
		else if (objData.EqualObjectName(L"Player") == true) {
			player = g_goMgr.NewAGO<Player>();
			player->SetPosition(objData.position);
			player->SetRotation(objData.rotation);
			//player->SetEnemysList(m_enemysList);
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
	itemBox = g_goMgr.NewAGO<ITEMBox>();
	for (auto enemy : m_enemysList) {
		enemy->SetPlayer(player);
	}
	player->SetEnemysList(m_enemysList);
	player->SetBox(itemBox);
	g_Camera = g_goMgr.NewAGO<GameCamera>();
	g_Camera->SetPlayer(player);
	hp_bar = g_goMgr.NewAGO<HPText>();
	hp_bar->SetPlayerHP(player->GetPlayerHP());
	energy_bar = g_goMgr.NewAGO<EnergyText>();
	energy_bar->SetPlayerEnergy(player->GetPlayerEnergy());
	return true;
}