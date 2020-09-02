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
	//ドアが消えていないなら。
	if (door != nullptr)
	{
		g_goMgr.QutavaleyaAGO(door);
	}
	////アイテムが消えていないなら。
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
		//DebugStage();
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
//最初のステージ
bool Game::FirstStage()
{
	//BGM
	m_bgm.Init(L"Assets/sound/FirstBGM.wav");
	m_bgm.Play(true);
	m_bgm.SetVolume(0.3f);
	mapLevel.Init(L"Assets/level/FirstStage.tkl",
		[&](LevelObjectData& objData)
	{
		//ロボット型の敵。蹴りを入れてくる。
		if (objData.ForwardMatchName(L"Robbot") == true) {
			//敵(一人目)のオブジェクト。
			Enemys* enemys = g_goMgr.NewAGO<Titan>();
			int num = _wtoi(&objData.name[6]);
			enemys->SetObjNum(num);
			enemys->SetPosition(objData.position);
			enemys->SetRotation(objData.rotation);
			//後で削除するのでリストに積んで記憶しておく。
			m_enemysToPlayerList.push_back(enemys);
			//フックしたのでtrueを返す。
			return true;
		}
		//ロボット型の敵。目からビーム。
		if (objData.ForwardMatchName(L"humanRobbot") == true) {
			//敵(一人目)のオブジェクト。
			Enemys* enemys = g_goMgr.NewAGO<StoneGolem>();
			int num = _wtoi(&objData.name[11]);
			enemys->SetObjNum(num);
			enemys->SetPosition(objData.position);
			enemys->SetRotation(objData.rotation);
			//後で削除するのでリストに積んで記憶しておく。
			m_enemysToPlayerList.push_back(enemys);
			//フックしたのでtrueを返す。
			return true;
		}
		//茶色のゴーレム。回転攻撃。
		if (objData.ForwardMatchName(L"Golem") == true) {
			//敵(一人目)のオブジェクト。
			Enemys* enemys = g_goMgr.NewAGO<Golem>();
			int num = _wtoi(&objData.name[5]);
			enemys->SetObjNum(num);
			enemys->SetPosition(objData.position);
			enemys->SetRotation(objData.rotation);
			//後で削除するのでリストに積んで記憶しておく。
			m_enemysToPlayerList.push_back(enemys);
			//フックしたのでtrueを返す。
			return true;
		}
		//存在しているだけの敵
		if (objData.ForwardMatchName(L"Jon") == true) {
			//敵(一人目)のオブジェクト。
			Enemys* enemys = g_goMgr.NewAGO<StoneEnemy>();
			int num = _wtoi(&objData.name[3]);
			enemys->SetObjNum(num);
			enemys->SetPosition(objData.position);
			enemys->SetRotation(objData.rotation);
			//後で削除するのでリストに積んで記憶しておく。
			m_enemysToPlayerList.push_back(enemys);
			//フックしたのでtrueを返す。
			return true;
		}
		if (objData.EqualObjectName(L"Player") == true) {
			player = g_goMgr.NewAGO<Player>();
			player->SetPosition(objData.position);
			player->SetRotation(objData.rotation);
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"Map") == true) {
			map = g_goMgr.NewAGO<MAP>();
			map->SetPosition(objData.position);
			map->SetStage(m_stage);
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"Box") == true) {
			itemBox = g_goMgr.NewAGO<ITEMBox>();
			itemBox->SetPosition(objData.position);
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.ForwardMatchName(L"Wall") == true) {
			m_wall = g_goMgr.NewAGO<Wall>();
			int num = _wtoi(&objData.name[4]);
			m_wall->SetObjNum(num);
			m_wall->SetPosition(objData.position);
			m_wall->SetRotation(objData.rotation);
			m_wallList[num] = m_wall;
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"Door") == true) {
			door = g_goMgr.NewAGO<Door>();
			door->SetPosition(objData.position);
			//door->SetPlayer(player);
			StageChange = true;
			//フックしたのでtrueを返す。
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
			m_enemysToPlayerList.push_back(enemys);
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"Player") == true) {
			player = g_goMgr.NewAGO<Player>();
			player->SetPosition(objData.position);
			player->SetRotation(objData.rotation);
			player->SetHP(m_playerHP);
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"MAP") == true) {
			map = g_goMgr.NewAGO<MAP>();
			map->SetPosition(objData.position);
			map->SetStage(1);
			//フックしたのでtrueを返す。
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
//デバック用ステージ
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
			//敵(一人目)のオブジェクト。
			Enemys* enemys = g_goMgr.NewAGO<Golem>();
			enemys->SetPosition(objData.position);
			enemys->SetRotation(objData.rotation);
			//enemys->SetScale(objData.scale);
			//後で削除するのでリストに積んで記憶しておく。
			m_enemysToPlayerList.push_back(enemys);

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
			m_enemysToPlayerList.push_back(enemys);
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
		else if (objData.EqualObjectName(L"debugmap") == true) {
			map = g_goMgr.NewAGO<MAP>();
			map->SetPosition(objData.position);
			map->SetStage(1);
			//フックしたのでtrueを返す。
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
//最初のステージで行うアップデート。
void Game::FirstStageUpdate()
{
	//HPとエナジー処理
	PlayerBarUpdate();
	//壁を消す処理。
	Walldelete();
	//ステージ移動とオーバーの処理
	GameScene();
}
//ボスのステージで行うアップデート。
void Game::BossStageUpdate()
{
	//HPとエナジー処理
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
//壁の処理。
void Game::Walldelete()
{
	int m_enemysNum[4] = { 0,0,0,0 };				//グループの人数
	for (auto enemy : m_enemysToPlayerList)
	{
		if (!enemy->GetIsDead())
		{
			m_enemysNum[enemy->GetObjData() - 1]++;
		}
	}
	//for文うける
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
//ゲーム遷移の処理
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
//プレイヤーの処理
void Game::PlayerBarUpdate()
{
	//HPとエナジー
	hp_bar->SetPlayerHP(player->GetPlayerHP());
	energy_bar->SetPlayerEnergy(player->GetPlayerEnergy());
}