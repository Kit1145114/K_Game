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

Game* Game::m_instance = nullptr;	//ゲームのインスタンスの生成

Game::Game()
{
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
	//ゲームのスタート関数呼び出し。
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
	////プレイヤーをNewGOで生成。
	//player = g_goMgr.NewAGO<Player>();
	////マップをNewGOで生成。
	//map = g_goMgr.NewAGO<MAP>();
	////ゲームカメラをNewGOで生成。
	//g_Camera = g_goMgr.NewAGO<GameCamera>();
	//g_Camera->SetPlayer(player);
	//g_Camera->SetEnemysList(m_goList);
	////エネミーを生成。
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
	hp_bar->SetPlayerHP(player->GetPlayerHP());
	energy_bar->SetPlayerEnergy(player->GetPlayerEnergy());
	for (auto enemy : m_enemysList) {
		if (enemy->GetIsDead())
		{
			Title* title = g_goMgr.NewAGO<Title>();
			g_goMgr.QutavaleyaAGO(this);
		}
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