#include"stdafx.h"
#include"Game.h"
#include"MAP.h"
#include"GameObjectManajer.h"
#include"Sample.h"
#include"HPText.h"
#include"EnergyText.h"
#include"ITEM/ITEMBox.h"
#include"Door.h"
#include"../Chara/Player.h"
#include"../Chara/Enemys.h"
#include"../Chara/StoneEnemy.h"
#include"../Chara/Titan.h"
#include"../Chara/Golem.h"
#include"../Chara/Boss.h"
#include"../GameSystem/Title.h"
#include"../GameSystem/GameCamera.h"
#include"../GameSystem/ChangeScreen.h"
#include"../GameSystem/GameClear.h"

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
	if (door != nullptr)
	{
		g_goMgr.QutavaleyaAGO(door);
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
	//�Q�[���̃X�^�[�g�֐��Ăяo���B
	switch (m_stage)
	{
	case First:
		FirstStage();
		break;
	case Second:
		NewBoss();
		break;
	}
	return true;
}
//�Q�[���̃A�b�v�f�[�g�B
void Game::Update()
{
	hp_bar->SetPlayerHP(player->GetPlayerHP());
	energy_bar->SetPlayerEnergy(player->GetPlayerEnergy());
	bool		isLive = false;
	for (auto enemy : m_enemysList) {
		if (!enemy->GetIsDead())
		{
			isLive = true;
		}
		else if (enemy->GetIsDead())
		{	
			m_enemysList.pop_back();
		}
	}
	if (!isLive && !StageChange)
	{
		door = g_goMgr.NewAGO<Door>();
		door->SetPlayer(player);
		StageChange = true;
	}
	else if (!isLive && StageChange) {
		if (door->GetChangeSta() && m_stage == First)
		{
			ChangeScreen* changescreen = g_goMgr.NewAGO<ChangeScreen>();
			g_goMgr.QutavaleyaAGO(this);
		}
		else if (door->GetChangeSta() && m_stage == Second)
		{
			GameClear* gameClear = g_goMgr.NewAGO<GameClear>();
			g_goMgr.QutavaleyaAGO(this);
		}
	}
}
//�ŏ��̃X�e�[�W
bool Game::FirstStage()
{
	//BGM
	m_bgm.Init(L"Assets/sound/FirstBGM.wav");
	m_bgm.Play(true);
	m_bgm.SetVolume(0.15f);
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
		//else if (objData.EqualObjectName(L"Enemy3") == true) {
		//	//�G(��l��)�̃I�u�W�F�N�g�B
		//	Enemys* enemys = g_goMgr.NewAGO<StoneEnemy>();
		//	enemys->SetPosition(objData.position);
		//	enemys->SetRotation(objData.rotation);
		//	//enemys->SetScale(objData.scale);
		//	//��ō폜����̂Ń��X�g�ɐς�ŋL�����Ă����B
		//	m_enemysList.push_back(enemys);
		//	//�t�b�N�����̂�true��Ԃ��B
		//	return true;
		//}
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
//�{�X�o���p
bool Game::NewBoss()
{
	//BGM
	m_bgm.Init(L"Assets/sound/BossBGM.wav");
	m_bgm.Play(true);
	m_bgm.SetVolume(0.15f);
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
	//itemBox = g_goMgr.NewAGO<ITEMBox>();
	for (auto enemy : m_enemysList) {
		enemy->SetPlayer(player);
	}
	player->SetEnemysList(m_enemysList);
	//player->SetBox(itemBox);
	g_Camera = g_goMgr.NewAGO<GameCamera>();
	g_Camera->SetPlayer(player);
	hp_bar = g_goMgr.NewAGO<HPText>();
	hp_bar->SetPlayerHP(player->GetPlayerHP());
	energy_bar = g_goMgr.NewAGO<EnergyText>();
	energy_bar->SetPlayerEnergy(player->GetPlayerEnergy());
	return true;
}