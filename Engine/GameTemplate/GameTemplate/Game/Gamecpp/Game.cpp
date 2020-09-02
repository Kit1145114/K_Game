#include"stdafx.h"
#include"Game.h"
#include"MAP.h"
#include"Sample.h"
#include"HPText.h"
#include"EnergyText.h"
#include"ITEM/ITEMBox.h"
#include"Door.h"
#include"Chara/Player.h"
#include"Chara/Enemys.h"
#include"Chara/StoneEnemy.h"
#include"Chara/Titan.h"
#include"Chara/Golem.h"
#include"Chara/Boss.h"
#include"GameSystem/Title.h"
#include"GameSystem/GameCamera.h"
#include"GameSystem/ChangeScreen.h"
#include"GameSystem/GameClear.h"
#include"GameSystem/GameOver.h"
#include"ITEM/Wall.h"
#include"Chara/StoneGolem.h"

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
	//�h�A�������Ă��Ȃ��Ȃ�B
	if (door != nullptr)
	{
		g_goMgr.QutavaleyaAGO(door);
	}
	////�A�C�e���������Ă��Ȃ��Ȃ�B
	if (itemBox != nullptr)
	{
		g_goMgr.QutavaleyaAGO(itemBox);
	}
	if (m_wall != nullptr)
	{
		g_goMgr.QutavaleyaAGO(m_wall);
	}
	for (auto wall : m_wallList) {
		if (wall.second != nullptr) {
			g_goMgr.QutavaleyaAGO(wall.second);
		}
	}
	for (auto enemy : m_enemysToPlayerList) {
		if (enemy != nullptr) {
			g_goMgr.QutavaleyaAGO(enemy);
		}
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
		//DebugStage();
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
	switch (m_stage)
	{
	case First:
		FirstStageUpdate();
		break;
	case Second:
		BossStageUpdate();
		break;
	}
}
//�ŏ��̃X�e�[�W
bool Game::FirstStage()
{
	//BGM
	m_bgm.Init(L"Assets/sound/FirstBGM.wav");
	m_bgm.Play(true);
	m_bgm.SetVolume(0.3f);
	mapLevel.Init(L"Assets/level/FirstStage.tkl",
		[&](LevelObjectData& objData)
	{
		//���{�b�g�^�̓G�B�R������Ă���B
		if (objData.ForwardMatchName(L"Robbot") == true) {
			//�G(��l��)�̃I�u�W�F�N�g�B
			Enemys* enemys = g_goMgr.NewAGO<Titan>();
			int num = _wtoi(&objData.name[6]);
			enemys->SetObjNum(num);
			enemys->SetPosition(objData.position);
			enemys->SetRotation(objData.rotation);
			//��ō폜����̂Ń��X�g�ɐς�ŋL�����Ă����B
			m_enemysToPlayerList.push_back(enemys);
			//�t�b�N�����̂�true��Ԃ��B
			return true;
		}
		//���{�b�g�^�̓G�B�ڂ���r�[���B
		if (objData.ForwardMatchName(L"humanRobbot") == true) {
			//�G(��l��)�̃I�u�W�F�N�g�B
			Enemys* enemys = g_goMgr.NewAGO<StoneGolem>();
			int num = _wtoi(&objData.name[11]);
			enemys->SetObjNum(num);
			enemys->SetPosition(objData.position);
			enemys->SetRotation(objData.rotation);
			//��ō폜����̂Ń��X�g�ɐς�ŋL�����Ă����B
			m_enemysToPlayerList.push_back(enemys);
			//�t�b�N�����̂�true��Ԃ��B
			return true;
		}
		//���F�̃S�[�����B��]�U���B
		if (objData.ForwardMatchName(L"Golem") == true) {
			//�G(��l��)�̃I�u�W�F�N�g�B
			Enemys* enemys = g_goMgr.NewAGO<Golem>();
			int num = _wtoi(&objData.name[5]);
			enemys->SetObjNum(num);
			enemys->SetPosition(objData.position);
			enemys->SetRotation(objData.rotation);
			//��ō폜����̂Ń��X�g�ɐς�ŋL�����Ă����B
			m_enemysToPlayerList.push_back(enemys);
			//�t�b�N�����̂�true��Ԃ��B
			return true;
		}
		//���݂��Ă��邾���̓G
		if (objData.ForwardMatchName(L"Jon") == true) {
			//�G(��l��)�̃I�u�W�F�N�g�B
			Enemys* enemys = g_goMgr.NewAGO<StoneEnemy>();
			int num = _wtoi(&objData.name[3]);
			enemys->SetObjNum(num);
			enemys->SetPosition(objData.position);
			enemys->SetRotation(objData.rotation);
			//��ō폜����̂Ń��X�g�ɐς�ŋL�����Ă����B
			m_enemysToPlayerList.push_back(enemys);
			//�t�b�N�����̂�true��Ԃ��B
			return true;
		}
		if (objData.EqualObjectName(L"Player") == true) {
			player = g_goMgr.NewAGO<Player>();
			player->SetPosition(objData.position);
			player->SetRotation(objData.rotation);
			//�t�b�N�����̂�true��Ԃ��B
			return true;
		}
		else if (objData.EqualObjectName(L"Map") == true) {
			map = g_goMgr.NewAGO<MAP>();
			map->SetPosition(objData.position);
			map->SetStage(m_stage);
			//�t�b�N�����̂�true��Ԃ��B
			return true;
		}
		else if (objData.EqualObjectName(L"Box") == true) {
			itemBox = g_goMgr.NewAGO<ITEMBox>();
			itemBox->SetPosition(objData.position);
			//�t�b�N�����̂�true��Ԃ��B
			return true;
		}
		else if (objData.ForwardMatchName(L"Wall") == true) {
			m_wall = g_goMgr.NewAGO<Wall>();
			int num = _wtoi(&objData.name[4]);
			m_wall->SetObjNum(num);
			m_wall->SetPosition(objData.position);
			m_wall->SetRotation(objData.rotation);
			m_wallList[num] = m_wall;
			//�t�b�N�����̂�true��Ԃ��B
			return true;
		}
		else if (objData.EqualObjectName(L"Door") == true) {
			door = g_goMgr.NewAGO<Door>();
			door->SetPosition(objData.position);
			//door->SetPlayer(player);
			StageChange = true;
			//�t�b�N�����̂�true��Ԃ��B
			return true;
		}
		return true;
	});
	door->SetPlayer(player);
	for (auto enemy : m_enemysToPlayerList) {
		enemy->SetPlayer(player);
	}
	g_Camera = g_goMgr.NewAGO<GameCamera>();
	g_Camera->SetPlayer(player);
	player->SetBox(itemBox);
	player->SetEnemysList(m_enemysToPlayerList);
	player->SetCamera(g_Camera);
	player->SetHP(m_playerHP);
	itemBox->SetPlayer(player);
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
			m_enemysToPlayerList.push_back(enemys);
			//�t�b�N�����̂�true��Ԃ��B
			return true;
		}
		else if (objData.EqualObjectName(L"Player") == true) {
			player = g_goMgr.NewAGO<Player>();
			player->SetPosition(objData.position);
			player->SetRotation(objData.rotation);
			player->SetHP(m_playerHP);
			//�t�b�N�����̂�true��Ԃ��B
			return true;
		}
		else if (objData.EqualObjectName(L"MAP") == true) {
			map = g_goMgr.NewAGO<MAP>();
			map->SetPosition(objData.position);
			map->SetStage(1);
			//�t�b�N�����̂�true��Ԃ��B
			return true;
		}
		return true;
	});
	//itemBox = g_goMgr.NewAGO<ITEMBox>();
	for (auto enemy : m_enemysToPlayerList) {
		enemy->SetPlayer(player);
	}
	g_Camera = g_goMgr.NewAGO<GameCamera>();
	g_Camera->SetPlayer(player);
	player->SetBox(itemBox);
	player->SetEnemysList(m_enemysToPlayerList);
	player->SetCamera(g_Camera);
	hp_bar = g_goMgr.NewAGO<HPText>();
	hp_bar->SetPlayerHP(player->GetPlayerHP());
	energy_bar = g_goMgr.NewAGO<EnergyText>();
	energy_bar->SetPlayerEnergy(player->GetPlayerEnergy());
	return true;
}
//�f�o�b�N�p�X�e�[�W
bool Game::DebugStage()
{
	//BGM
	m_bgm.Init(L"Assets/sound/FirstBGM.wav");
	m_bgm.Play(true);
	m_bgm.SetVolume(0.15f);
	mapLevel.Init(L"Assets/level/debugStage.tkl",
		[&](LevelObjectData& objData)
	{
		if (objData.EqualObjectName(L"Enemy2") == true) {
			//�G(��l��)�̃I�u�W�F�N�g�B
			Enemys* enemys = g_goMgr.NewAGO<Golem>();
			enemys->SetPosition(objData.position);
			enemys->SetRotation(objData.rotation);
			//enemys->SetScale(objData.scale);
			//��ō폜����̂Ń��X�g�ɐς�ŋL�����Ă����B
			m_enemysToPlayerList.push_back(enemys);

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
			m_enemysToPlayerList.push_back(enemys);
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
		else if (objData.EqualObjectName(L"debugmap") == true) {
			map = g_goMgr.NewAGO<MAP>();
			map->SetPosition(objData.position);
			map->SetStage(1);
			//�t�b�N�����̂�true��Ԃ��B
			return true;
		}
		return true;
	});
	for (auto enemy : m_enemysToPlayerList) {
		enemy->SetPlayer(player);
	}
	itemBox = g_goMgr.NewAGO<ITEMBox>();
	m_wall = g_goMgr.NewAGO<Wall>();
	player->SetBox(itemBox);
	player->SetEnemysList(m_enemysToPlayerList);
	g_Camera = g_goMgr.NewAGO<GameCamera>();
	g_Camera->SetPlayer(player);
	hp_bar = g_goMgr.NewAGO<HPText>();
	hp_bar->SetPlayerHP(player->GetPlayerHP());
	energy_bar = g_goMgr.NewAGO<EnergyText>();
	energy_bar->SetPlayerEnergy(player->GetPlayerEnergy());
	return true;
}
//�ŏ��̃X�e�[�W�ōs���A�b�v�f�[�g�B
void Game::FirstStageUpdate()
{
	//HP�ƃG�i�W�[����
	PlayerBarUpdate();
	//�ǂ����������B
	Walldelete();
	//�X�e�[�W�ړ��ƃI�[�o�[�̏���
	GameScene();
}
//�{�X�̃X�e�[�W�ōs���A�b�v�f�[�g�B
void Game::BossStageUpdate()
{
	//HP�ƃG�i�W�[����
	PlayerBarUpdate();
	bool		isLive = false;
	for (auto enemy : m_enemysToPlayerList) {
		if (!enemy->GetIsDead())
		{
			isLive = true;
		}
		else if (enemy->GetIsDead())
		{
			m_enemysToPlayerList.pop_back();
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
	if (player->GetPlayerHP() <= ZERO)
	{
		GameOver* gameover = g_goMgr.NewAGO<GameOver>();
		g_goMgr.QutavaleyaAGO(this);
	}
}
//�ǂ̏����B
void Game::Walldelete()
{
	int m_enemysNum[4] = { 0,0,0,0 };				//�O���[�v�̐l��
	for (auto enemy : m_enemysToPlayerList)
	{
		if (!enemy->GetIsDead())
		{
			m_enemysNum[enemy->GetObjData() - 1]++;
		}
	}
	//for��������
	for (int i = 0; i < 4; i++) {
		if (m_enemysNum[i] == 0) {
			if (m_wallList[i+1] != nullptr) {
				//g_goMgr.QutavaleyaAGO(m_wallList[i+1]);
				m_wallList[i + 1]->SetMoveflag(true);
				m_wallList[i+1] = nullptr;
			}
		}
	}
}
//�Q�[���J�ڂ̏���
void Game::GameScene()
{
	if (door->GetChangeSta() && m_stage == First)
	{
		ChangeScreen* changescreen = g_goMgr.NewAGO<ChangeScreen>();
		changescreen->SetPlayerHP(player->GetPlayerHP());
		g_goMgr.QutavaleyaAGO(this);
	}
	if (player->GetIsDead())
	{
		GameOver* gameover = g_goMgr.NewAGO<GameOver>();
		g_goMgr.QutavaleyaAGO(this);
	}
}
//�v���C���[�̏���
void Game::PlayerBarUpdate()
{
	//HP�ƃG�i�W�[
	hp_bar->SetPlayerHP(player->GetPlayerHP());
	energy_bar->SetPlayerEnergy(player->GetPlayerEnergy());
}