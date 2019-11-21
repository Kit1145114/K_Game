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
	Start();			
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
	for (int i = 0; i < enemyNum; i++)
	{
		if (enemys[i] != nullptr)
		{
			//エネミーを削除。
			g_goMgr.QutavaleyaAGO(enemys[i]);
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
	//プレイヤーをNewGOで生成。
	player = g_goMgr.NewAGO<Player>();
	//マップをNewGOで生成。
	map = g_goMgr.NewAGO<MAP>();
	//ゲームカメラをNewGOで生成。
	g_Camera = g_goMgr.NewAGO<GameCamera>();
	g_Camera->SetPlayer(player);
	//エネミーを生成。
	enemys[0] = g_goMgr.NewAGO<StoneEnemy>();
	enemys[1] = g_goMgr.NewAGO<Golem>();
	enemys[2] = g_goMgr.NewAGO<Titan>();
	enemys[3] = g_goMgr.NewAGO<Sample>();
	enemys[3]->SetPlayer(player);
	//hp_bar = g_goMgr.NewAGO<HPText>();
	return true;
}
//ゲームのアップデート。
void Game::Update()
{
}
