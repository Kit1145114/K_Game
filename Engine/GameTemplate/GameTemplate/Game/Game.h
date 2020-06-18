#pragma once
#include"graphics/c3dmodel/C3DModelDraw.h"
#include"RenderTarget.h"
#include"level/Level.h"
#include"GameSystem/GameConst.h"
#include"sound/SoundEngine.h"
#include"sound/SoundSource.h"

class Player;					//プレイヤー用のclassの型を作成。
class MAP;						//マップ用のclassの型を生成。
class GameCamera;				//カメラ用のclassの型を生成。
class Enemys;					//エネミー用のclassの型を生成。
class HPText;					//HPバー用のclass
class EnergyText;				//エナジーバー用のclass
class ITEMBox;					//箱のclass
class Door;						//ドアのclassノ型
class Wall;						//壁の型
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
	bool FirstStage();				//最初のステージ
	bool NewBoss();					//ボスを呼ぶ関数
	bool DebugStage();				//デバック用のステージ。
	void Update();					//ゲームのアップデート関数。
	void FirstStageUpdate();		//最初のステージで行うアップデート。
	void BossStageUpdate();			//ボスのステージで行うアップでーろ。
	//敵とぷえいやーの距離を測る
public:
	void SetStage(int n)
	{
		m_stage = n;
	}

private:
	Player* player = nullptr;					//プレイヤーのインスタンス。
	MAP* map = nullptr;						//マップのインスタンス。
	GameCamera* g_Camera = nullptr;			//カメラのインスタンス。
	Enemys* enemys = nullptr;					//エネミーのインスタンス。
	HPText* hp_bar = nullptr;					//HPバー。
	ITEMBox* itemBox = nullptr;				//箱
	EnergyText* energy_bar = nullptr;			//エナジーバー
	Door* door = nullptr;						//ドア
	Wall* m_wall = nullptr;					//壁
	Level		mapLevel;			//マップのレベル。。。。。。
	StageNum	stage;
	bool StageChange = false;
	int	m_stage = 0;
	int m_stagenum = 0;
	//bool		isLive = false;
	static Game* m_instance;		//ゲームのインスタンス。
	const int ENEMY_NUM = 4;		//登場するエネミーの数をここで定義。
		//可変長配列
	std::vector<Enemys*>m_enemysList;	//エネミーオブジェクトのリスト
	std::vector<Wall*>m_wallList;		//壁のリスト
	CVector3 m_initPlayerPos = { 0.0f,500.0f,0.0f };
	//BGM
	CSoundSource m_bgm;
};