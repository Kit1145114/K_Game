#pragma once
#include"GameObjectManajer.h"
#include"graphics/c3dmodel/C3DModelDraw.h"
#include"RenderTarget.h"
#include"level/Level.h"
#include"GameConst.h"

class Player;					//プレイヤー用のclassの型を作成。
class MAP;						//マップ用のclassの型を生成。
class GameCamera;				//カメラ用のclassの型を生成。
class Enemys;					//エネミー用のclassの型を生成。
class HPText;
class AgoSample;

class Game : public GameObject
{

	enum EnRenderMode {
		enRenderMode_Normal,		//通常レンダリング。
		enRenderMode_Silhouette,	//シルエットをレンダリング。
		enRenderMode_Num,			//レンダリングモードの数。
	};
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Game();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Game();
	static Game* GetInstance();		//シングルトン。
	bool Start();					//ゲームのスタート関数。
	void Update();					//ゲームのアップデート関数。
	//敵とぷえいやーの距離を測る
private:
	Player* player;					//プレイヤーのインスタンス。
	MAP* map;						//マップのインスタンス。
	GameCamera* g_Camera;			//カメラのインスタンス。
	Enemys* enemys;					//エネミーのインスタンス。
	HPText* hp_bar;					//HPバー。
	AgoSample* ago;					//顎。
	Level		mapLevel;			//マップのレベル。。。。。。
	static Game* m_instance;		//ゲームのインスタンス。
	const int ENEMY_NUM = 4;		//登場するエネミーの数をここで定義。
		//可変長配列
	std::vector<Enemys*>m_enemysList;	//エネミーオブジェクトのリスト
	CVector3 m_initPlayerPos = { 0.0f,500.0f,0.0f };
};

