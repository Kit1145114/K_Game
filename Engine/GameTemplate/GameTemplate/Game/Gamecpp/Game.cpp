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

Game* Game::m_instance = nullptr;	//ゲームのインスタンスの生成

Game::Game()
{
	m_soundEngine.Init();
	m_bgm.Init(L"Assets/sound/Result.wav");
	m_bgm.Play(true);
	m_bgm.SetVolume(0.15f);
	//もし、ゲームが既に存在していたら
	if (m_instance != nullptr)
	{
		//ゲームを終了させる。
		std::abort();
	}
	else {
		//何もない場合はゲームを代入。
		m_instance = this;
	}
	////ゲームのスタート関数呼び出し。
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
	//ゲームの削除とともに、インスタンスにnullを入れる。
	m_instance = nullptr;	
	//もしプレイヤーが消えてなかったら
	if (player != nullptr)
	{
		//プレイヤーを削除。
		g_goMgr.QutavaleyaAGO(player);
	}
	//もし、マップが消えてなかったら
	if (map != nullptr)
	{
		//マップを削除。
		g_goMgr.QutavaleyaAGO(map);
	}
	//もしエネミーが消えてなかったら。
	for (auto enemy : m_enemysList) {
		if (enemy != nullptr) {
			g_goMgr.QutavaleyaAGO(enemy);
		}
	}
	//カメラ削除
	if (g_Camera != nullptr)
	{
		g_goMgr.QutavaleyaAGO(g_Camera);
	}
	//HPバー削除
	if (hp_bar != nullptr) {
		g_goMgr.QutavaleyaAGO(hp_bar);
	}
	//エナジーバー削除
	if (energy_bar != nullptr) {
		g_goMgr.QutavaleyaAGO(energy_bar);
	}
}


Game* Game::GetInstance()
{
	//どこからでもゲームが呼び出せるように
	//インスタンスをreturnで返す。
	return m_instance;
}
//ゲーム開始ィィィィ！！！！。
bool Game::Start()
{
	mapLevel.Init(L"Assets/level/FirstStage.tkl",
		[&](LevelObjectData& objData)
	{
		if (objData.EqualObjectName(L"Enemy2") == true) {
			//敵(一人目)のオブジェクト。
			Enemys* enemys = g_goMgr.NewAGO<Golem>();
			enemys->SetPosition(objData.position);
			enemys->SetRotation(objData.rotation);
			//enemys->SetScale(objData.scale);
			//後で削除するのでリストに積んで記憶しておく。
			m_enemysList.push_back(enemys);
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"Enemy3") == true) {
			//敵(二人目)のオブジェクト。
			Enemys* enemys = g_goMgr.NewAGO<StoneEnemy>();
			enemys->SetPosition(objData.position);
			enemys->SetRotation(objData.rotation);
			//enemys->SetScale(objData.scale);
			//後で削除するのでリストに積んで記憶しておく。
			m_enemysList.push_back(enemys);
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"RobbotEnemy1") == true) {
			//敵(二人目)のオブジェクト。
			enemys = g_goMgr.NewAGO<Titan>();
			enemys->SetPosition(objData.position);
			enemys->SetRotation(objData.rotation);
			//enemys->SetScale(objData.scale);
			//後で削除するのでリストに積んで記憶しておく。
			m_enemysList.push_back(enemys);
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"Player") == true) {
			player = g_goMgr.NewAGO<Player>();
			player->SetPosition(objData.position);
			player->SetRotation(objData.rotation);
			//player->SetEnemysList(m_enemysList);
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"MAP") == true) {
			map = g_goMgr.NewAGO<MAP>();
			map->SetPosition(objData.position);
			//フックしたのでtrueを返す。
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
//ゲームのアップデート。
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
//ボス出現用
bool Game::NewBoss()
{
	mapLevel.Init(L"Assets/level/BossStage.tkl",
		[&](LevelObjectData& objData)
	{
		if (objData.EqualObjectName(L"RobbotBoss") == true) {
			//敵(一人目)のオブジェクト。
			Enemys* enemys = g_goMgr.NewAGO<Boss>();
			enemys->SetPosition(objData.position);
			enemys->SetRotation(objData.rotation);
			//後で削除するのでリストに積んで記憶しておく。
			m_enemysList.push_back(enemys);
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"Player") == true) {
			player = g_goMgr.NewAGO<Player>();
			player->SetPosition(objData.position);
			player->SetRotation(objData.rotation);
			//player->SetEnemysList(m_enemysList);
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"MAP") == true) {
			map = g_goMgr.NewAGO<MAP>();
			map->SetPosition(objData.position);
			//フックしたのでtrueを返す。
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