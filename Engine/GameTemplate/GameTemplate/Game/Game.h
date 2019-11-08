#pragma once
#include"GameObjectManajer.h"
#include"Enemys.h"

class Player;					//プレイヤー用のclassの型を作成。
class MAP;						//マップ用のclassの型を生成。
class GameCamera;				//カメラ用のclassの型を生成。
class Enemys;					//エネミー用のclassの型を生成。
class StoneEnemy;				//仮エネミー。

class Game : public GameObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Game();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Game();
	static Game* GetInstance();	//シングルトン。
	bool Start();				//ゲームのスタート関数。
	void Update();				//ゲームのアップデート関数。
private:
	Player* player;				//プレイヤーのインスタンス。
	MAP* map;					//マップのインスタンス。
	GameCamera* g_Camera;		//カメラのインスタンス。
	Enemys* enemys[4];			//エネミーのインスタンス。
	static Game* m_instance;	//ゲームのインスタンス。
	int enemyNum = 3;			//登場するエネミーの数をここで定義。
};

