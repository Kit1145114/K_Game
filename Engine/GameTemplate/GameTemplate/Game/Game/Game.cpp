#include"stdafx.h"
#include"Game.h"
#include"Text/HPText.h"
#include"Text/EnergyText.h"
#include"Chara/Player.h"
#include"Chara/Enemys.h"
#include"Chara/Titan.h"
#include"Chara/Golem.h"
#include"Chara/Boss.h"
#include"Chara/StoneGolem.h"
#include"GameSystem/Title.h"
#include"GameSystem/GameCamera.h"
#include"GameSystem/ChangeScreen.h"
#include"GameSystem/GameClear.h"
#include"GameSystem/GameOver.h"
#include"Object/MAP.h"
#include"Object/ITEMBox.h"
#include"Object/Door.h"
#include"Object/Wall.h"
#include"Object/Sky.h"

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
	if (m_player != nullptr)
	{
		//プレイヤーを削除。
		g_goMgr.DeleteGO(m_player);
	}
	//もし、マップが消えてなかったら
	if (m_map != nullptr)
	{
		//マップを削除。
		g_goMgr.DeleteGO(m_map);
	}
	//カメラ削除
	if (m_camera != nullptr)
	{
		g_goMgr.DeleteGO(m_camera);
	}
	//HPバー削除
	if (m_hpText != nullptr) {
		g_goMgr.DeleteGO(m_hpText);
	}
	//エナジーバー削除
	if (m_energyText != nullptr) {
		g_goMgr.DeleteGO(m_energyText);
	}
	//ドアが消えていないなら。
	if (m_door != nullptr)
	{
		g_goMgr.DeleteGO(m_door);
	}
	////アイテムが消えていないなら。
	if (m_itemBox != nullptr)
	{
		g_goMgr.DeleteGO(m_itemBox);
	}
	if (m_wall != nullptr)
	{
		g_goMgr.DeleteGO(m_wall);
	}
	if (m_sky != nullptr)
	{
		g_goMgr.DeleteGO(m_sky);
	}
	for (auto wall : m_wallList) {
		if (wall.second != nullptr) {
			g_goMgr.DeleteGO(wall.second);
		}
	}
	for (auto enemy : m_enemysToPlayerList) {
		if (enemy != nullptr) {
			g_goMgr.DeleteGO(enemy);
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
			auto* m_enemys = g_goMgr.NewGO<Titan>();
			int num = _wtoi(&objData.name[6]);
			m_enemys->SetObjNum(num);
			m_enemys->SetPosition(objData.position);
			m_enemys->SetRotation(objData.rotation);
			//後で削除するのでリストに積んで記憶しておく。
			m_enemysToPlayerList.push_back(m_enemys);
			//フックしたのでtrueを返す。
			return true;
		}
		//ロボット型の敵。目からビーム。
		if (objData.ForwardMatchName(L"humanRobbot") == true) {
			//敵(一人目)のオブジェクト。
			auto* m_enemys = g_goMgr.NewGO<StoneGolem>();
			int num = _wtoi(&objData.name[11]);
			m_enemys->SetObjNum(num);
			m_enemys->SetPosition(objData.position);
			m_enemys->SetRotation(objData.rotation);
			//後で削除するのでリストに積んで記憶しておく。
			m_enemysToPlayerList.push_back(m_enemys);
			//フックしたのでtrueを返す。
			return true;
		}
		//茶色のゴーレム。回転攻撃。
		if (objData.ForwardMatchName(L"Golem") == true) {
			//敵(一人目)のオブジェクト。
			auto* m_enemys = g_goMgr.NewGO<Golem>();
			int num = _wtoi(&objData.name[5]);
			m_enemys->SetObjNum(num);
			m_enemys->SetPosition(objData.position);
			m_enemys->SetRotation(objData.rotation);
			//後で削除するのでリストに積んで記憶しておく。
			m_enemysToPlayerList.push_back(m_enemys);
			//フックしたのでtrueを返す。
			return true;
		}
		if (objData.EqualObjectName(L"Player") == true) {
			m_player = g_goMgr.NewGO<Player>();
			m_player->SetPosition(objData.position);
			m_player->SetRotation(objData.rotation);
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"Map") == true) {
			m_map = g_goMgr.NewGO<MAP>();
			m_map->SetPosition(objData.position);
			m_map->SetStage(m_stage);
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"Sky") == true) {
			m_sky = g_goMgr.NewGO<Sky>();
			m_sky->SetPosition(objData.position);
			//m_sky->SetScale(objData.scale);
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"Box") == true) {
			m_itemBox = g_goMgr.NewGO<ITEMBox>();
			m_itemBox->SetPosition(objData.position);
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.ForwardMatchName(L"Wall") == true) {
			m_wall = g_goMgr.NewGO<Wall>();
			int num = _wtoi(&objData.name[4]);
			m_wall->SetObjNum(num);
			m_wall->SetPosition(objData.position);
			m_wall->SetRotation(objData.rotation);
			m_wallList[num] = m_wall;
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"Door") == true) {
			m_door = g_goMgr.NewGO<Door>();
			m_door->SetPosition(objData.position);
			//door->SetPlayer(player);
			stageChange_flag = true;
			//フックしたのでtrueを返す。
			return true;
		}
		return true;
	});
	m_door->SetPlayer(m_player);
	for (auto enemy : m_enemysToPlayerList) {
		//プレイヤーのリスナーとして関連付けをする。
		enemy->BindPlayer(m_player);
	}
	m_camera = g_goMgr.NewGO<GameCamera>();
	m_camera->SetPlayer(m_player);
	m_player->SetBox(m_itemBox);
	m_player->SetEnemysList(m_enemysToPlayerList);
	m_player->SetCamera(m_camera);
	m_player->SetHP(m_playerHp);
	m_player->AddEventListener(this);
	m_itemBox->BindPlayer(m_player);
	m_hpText = g_goMgr.NewGO<HPText>();
	m_hpText->SetPlayerHP(m_player->GetPlayerHP());
	m_energyText = g_goMgr.NewGO<EnergyText>();
	m_energyText->SetPlayerEnergy(m_player->GetPlayerEnergy());

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
			Enemys* enemys = g_goMgr.NewGO<Boss>();
			enemys->SetPosition(objData.position);
			enemys->SetRotation(objData.rotation);
			//後で削除するのでリストに積んで記憶しておく。
			m_enemysToPlayerList.push_back(enemys);
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"Player") == true) {
			m_player = g_goMgr.NewGO<Player>();
			m_player->SetPosition(objData.position);
			m_player->SetRotation(objData.rotation);
			m_player->SetHP(m_playerHp);
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"MAP") == true) {
			m_map = g_goMgr.NewGO<MAP>();
			m_map->SetPosition(objData.position);
			m_map->SetStage(1);
			//フックしたのでtrueを返す。
			return true;
		}
		return true;
	});
	for (auto enemy : m_enemysToPlayerList) {
		enemy->BindPlayer(m_player);
	}
	m_camera = g_goMgr.NewGO<GameCamera>();
	m_camera->SetPlayer(m_player);
	m_player->SetBox(m_itemBox);
	m_player->SetEnemysList(m_enemysToPlayerList);
	m_player->SetCamera(m_camera);
	m_player->AddEventListener(this);
	m_hpText = g_goMgr.NewGO<HPText>();
	m_hpText->SetPlayerHP(m_player->GetPlayerHP());
	m_energyText = g_goMgr.NewGO<EnergyText>();
	m_energyText->SetPlayerEnergy(m_player->GetPlayerEnergy());
	m_sky = g_goMgr.NewGO<Sky>();
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
			auto* m_enemys = g_goMgr.NewGO<Golem>();
			m_enemys->SetPosition(objData.position);
			m_enemys->SetRotation(objData.rotation);
			//enemys->SetScale(objData.scale);
			//後で削除するのでリストに積んで記憶しておく。
			m_enemysToPlayerList.push_back(m_enemys);

			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"RobbotEnemy1") == true) {
			//敵(二人目)のオブジェクト。
			m_enemys = g_goMgr.NewGO<Titan>();
			m_enemys->SetPosition(objData.position);
			m_enemys->SetRotation(objData.rotation);
			//enemys->SetScale(objData.scale);
			//後で削除するのでリストに積んで記憶しておく。
			m_enemysToPlayerList.push_back(m_enemys);
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"Player") == true) {
			m_player = g_goMgr.NewGO<Player>();
			m_player->SetPosition(objData.position);
			m_player->SetRotation(objData.rotation);
			//player->SetEnemysList(m_enemysList);
			//フックしたのでtrueを返す。
			return true;
		}
		else if (objData.EqualObjectName(L"debugmap") == true) {
			m_map = g_goMgr.NewGO<MAP>();
			m_map->SetPosition(objData.position);
			m_map->SetStage(1);
			//フックしたのでtrueを返す。
			return true;
		}
		return true;
	});
	for (auto enemy : m_enemysToPlayerList) {
		enemy->BindPlayer(m_player);
	}
	m_itemBox = g_goMgr.NewGO<ITEMBox>();
	m_wall = g_goMgr.NewGO<Wall>();
	m_player->SetBox(m_itemBox);
	m_player->SetEnemysList(m_enemysToPlayerList);
	m_player->AddEventListener(this);
	m_camera = g_goMgr.NewGO<GameCamera>();
	m_camera->SetPlayer(m_player);
	m_hpText = g_goMgr.NewGO<HPText>();
	m_hpText->SetPlayerHP(m_player->GetPlayerHP());
	m_energyText = g_goMgr.NewGO<EnergyText>();
	m_energyText->SetPlayerEnergy(m_player->GetPlayerEnergy());
	return true;
}
void Game::OnPlayerDead(Player* pl)
{
	GameOver* gameover = g_goMgr.NewGO<GameOver>();
	g_goMgr.DeleteGO(this);
	pl->DeleteEvenetListener(this);
}
//最初のステージで行うアップデート。
void Game::FirstStageUpdate()
{
	//HPとエナジー処理
	PlayerBarUpdate();
	//壁を消す処理。
	Walldelete();
	//ステージ遷移
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
	if (!isLive && !stageChange_flag)
	{
		m_door = g_goMgr.NewGO<Door>();
		m_door->SetPlayer(m_player);
		m_door->SetPosition(m_doorPos);
		stageChange_flag = true;
	}
	else if (!isLive && stageChange_flag) {
		if (m_door->GetChangeSta() && m_stage == First)
		{
			ChangeScreen* changescreen = g_goMgr.NewGO<ChangeScreen>();
			g_goMgr.DeleteGO(this);
		}
		else if (m_door->GetChangeSta() && m_stage == Second)
		{
			GameClear* gameClear = g_goMgr.NewGO<GameClear>();
			g_goMgr.DeleteGO(this);
		}
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
	//for文
	for (int i = 0; i < 4; i++) {
		if (m_enemysNum[i] == 0) {
			if (m_wallList[i+1] != nullptr) {
				m_wallList[i + 1]->SetMoveflag(true);
				m_wallList[i+1] = nullptr;
			}
		}
	}
}
//ゲーム遷移の処理
void Game::GameScene()
{
	if (m_door->GetChangeSta() && m_stage == First)
	{
		ChangeScreen* changescreen = g_goMgr.NewGO<ChangeScreen>();
		changescreen->SetPlayerHP(m_player->GetPlayerHP());
		g_goMgr.DeleteGO(this);
	}
}
//プレイヤーの処理
void Game::PlayerBarUpdate()
{
	//HPとエナジー
	m_hpText->SetPlayerHP(m_player->GetPlayerHP());
	m_energyText->SetPlayerEnergy(m_player->GetPlayerEnergy());
}