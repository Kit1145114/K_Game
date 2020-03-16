#pragma once
#include"GameObjectManajer.h"
#include"graphics/c3dmodel/C3DModelDraw.h"
#include"RenderTarget.h"
#include"level/Level.h"
#include"GameConst.h"
#include"sound/SoundEngine.h"
#include"sound/SoundSource.h"

class Player;					//プレイヤー用のclassの型を作成。
class MAP;						//マップ用のclassの型を生成。
class GameCamera;				//カメラ用のclassの型を生成。
class Enemys;					//エネミー用のclassの型を生成。
class HPText;					//HPバー用のclass
class EnergyText;				//エナジーバー用のclass
class ITEMBox;					//箱のclass
class Door;
class Game : public GameObject
{

	enum EnRenderMode {
		enRenderMode_Normal,		//通常レンダリング。
		enRenderMode_Silhouette,	//シルエットをレンダリング。
		enRenderMode_Num,			//レンダリングモードの数。
	};
	//ステージの振り分け。
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
	bool NewBoss();					//ボスを呼ぶ関数
	void Update();					//ゲームのアップデート関数。
	//bool isLive = false;			//生きてるかどうか
	//敵とぷえいやーの距離を測る
private:
	Player* player;					//プレイヤーのインスタンス。
	MAP* map;						//マップのインスタンス。
	GameCamera* g_Camera;			//カメラのインスタンス。
	Enemys* enemys;					//エネミーのインスタンス。
	HPText* hp_bar;					//HPバー。
	ITEMBox* itemBox;				//箱
	EnergyText* energy_bar;			//エナジーバー
	Door*	door;					//ドア
	Level		mapLevel;			//マップのレベル。。。。。。
	int			m_stage;			//ステージ
	bool		StageChange = false;
	//bool		isLive = false;
	static Game* m_instance;		//ゲームのインスタンス。
	const int ENEMY_NUM = 4;		//登場するエネミーの数をここで定義。
		//可変長配列
	std::vector<Enemys*>m_enemysList;	//エネミーオブジェクトのリスト
	std::list<Enemys*>enemysList;		//エネミーのリスト。
	CVector3 m_initPlayerPos = { 0.0f,500.0f,0.0f };
	CSoundEngine m_soundEngine;
	CSoundSource m_bgm;
};

