#pragma once
#include<unordered_map>
#include"graphics/c3dmodel/C3DModelDraw.h"
#include"graphics/RenderTarget.h"
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
class Sky;						//空のclassの型。

class Game : public GameObject
{

	enum EnRenderMode {
		enRenderMode_Normal,		//通常レンダリング。
		enRenderMode_Silhouette,	//シルエットをレンダリング。
		enRenderMode_Num,			//レンダリングモードの数。
	};

	enum EnemyType {
		RobbotoEnemy1,
		RobbotoEnemy2,
		treeGolem,
		Jon,
		BossEnemy
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
	void Walldelete();				//壁の処理
	void GameScene();				//ゲームオーバー等の処理。
	void PlayerBarUpdate();			//プレイヤーの体力、ブーストのアプデ。
	//敵とぷえいやーの距離を測る
public:
	void SetStage(int n)
	{
		m_stage = n;
	}
	void SetPlayerHp(float hp)
	{
		m_playerHP = hp;
	}
private:
	Player* player = nullptr;				//プレイヤーのインスタンス。
	MAP* map = nullptr;						//マップのインスタンス。
	GameCamera* g_Camera = nullptr;			//カメラのインスタンス。
	Enemys* enemys = nullptr;				//エネミーのインスタンス。
	HPText* hp_bar = nullptr;				//HPバー。
	ITEMBox* itemBox = nullptr;				//箱
	EnergyText* energy_bar = nullptr;		//エナジーバー
	Door* door = nullptr;					//ドア
	Wall* m_wall = nullptr;					//壁
	Sky* m_sky = nullptr;					//空
	Level		mapLevel;					//マップのレベル。。。。。。
	StageNum	stage;
	bool StageChange = false;
	bool wallDeath_flag = false;			//壁削除のフラグ。
	bool deathEnemys1_Flag = false;			//グループ１全員死んだか
	bool deathEnemys2_Flag = false;			//グループ２全員死んだか
	bool deathEnemys3_Flag = false;			//グループ３全員死んだか
	bool deathEnemys4_Flag = false;			//グループ４全員死んだか
	bool isWallDelete_Flag[4] = {false,false,false,false};
	int	m_stage = 0;						//進むステージ
	int m_stagenum = 0;						//セットするステージの番号
	int m_maxEnemys[4] = { 0 };				//グループの最大人数
	float m_playerHP = 100.0f;				//プレイヤーの初期HP
	//bool		isLive = false;
	static Game* m_instance;		//ゲームのインスタンス。
	const int ENEMY_NUM = 4;		//登場するエネミーの数をここで定義。
		//可変長配列
	std::vector<Enemys*>m_enemysToPlayerList;	//壁用のエネミーオブジェクトのリスト
	std::unordered_map<int,Wall*>m_wallList;				//壁のリスト
	CVector3 m_initPlayerPos = { 0.0f,500.0f,0.0f };
	CVector3 DoorPos = { 0.0f,-350.0f,0.0f };
	//BGM
	CSoundSource m_bgm;
	//
	EnemyType e_type;
};