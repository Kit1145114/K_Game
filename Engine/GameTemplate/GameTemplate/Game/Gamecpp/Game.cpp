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
	if (door != nullptr)
	{
		g_goMgr.QutavaleyaAGO(door);
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
	//ゲームのスタート関数呼び出し。
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
//ゲームのアップデート。
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
//最初のステージ
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
		//else if (objData.EqualObjectName(L"Enemy3") == true) {
		//	//敵(二人目)のオブジェクト。
		//	Enemys* enemys = g_goMgr.NewAGO<StoneEnemy>();
		//	enemys->SetPosition(objData.position);
		//	enemys->SetRotation(objData.rotation);
		//	//enemys->SetScale(objData.scale);
		//	//後で削除するのでリストに積んで記憶しておく。
		//	m_enemysList.push_back(enemys);
		//	//フックしたのでtrueを返す。
		//	return true;
		//}
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
//ボス出現用
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