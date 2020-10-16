#pragma once
#include<unordered_map>
#include"graphics/c3dmodel/C3DModelDraw.h"
#include"graphics/RenderTarget.h"
#include"level/Level.h"
#include"GameSystem/GameConst.h"
#include"sound/SoundEngine.h"
#include"sound/SoundSource.h"
#include"Chara/IPlayerEventListener.h"

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

class Game : public GameObject, IPlayerEventListener
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
	const void SetStage(int n) 
	{
		m_stage = n;
	}
	const void SetPlayerHp(float hp) 
	{
		m_playerHp = hp;
	}
	/// <summary>
	/// プレイヤーが死亡したときに呼ばれる処理。
	/// </summary>
	/// <param name="pl"></param>
	void OnPlayerDead(Player* pl) override;
	/// <summary>
private:
	Player* m_player = nullptr;				//プレイヤーのインスタンス。
	MAP* m_map = nullptr;					//マップのインスタンス。
	GameCamera* m_camera = nullptr;			//カメラのインスタンス。
	Enemys* m_enemys = nullptr;				//エネミーのインスタンス。
	HPText* m_hpText = nullptr;				//HPバー。
	ITEMBox* m_itemBox = nullptr;			//箱
	EnergyText* m_energyText = nullptr;		//エナジーバー
	Door* m_door = nullptr;					//ドア
	Wall* m_wall = nullptr;					//壁
	Sky* m_sky = nullptr;					//空
	Level		mapLevel;					//マップのレベル。。。。。。
	StageNum	stage;
	bool stageChange_flag = false;
	bool wallDeath_flag = false;			//壁削除のフラグ。
	bool deathEnemys1_Flag = false;			//グループ１全員死んだか
	bool deathEnemys2_Flag = false;			//グループ２全員死んだか
	bool deathEnemys3_Flag = false;			//グループ３全員死んだか
	bool deathEnemys4_Flag = false;			//グループ４全員死んだか
	bool isWallDelete_Flag[4] = {false,false,false,false};
	int	m_stage = 0;						//進むステージ
	int m_stageNum = 0;						//セットするステージの番号
	int m_maxEnemys[4] = { 0 };				//グループの最大人数
	float m_playerHp = 100.0f;				//プレイヤーの初期HP
	static Game* m_instance;				//ゲームのインスタンス。
	const int ENEMY_NUM = 4;				//登場するエネミーの数をここで定義。
		//可変長配列
	std::vector<Enemys*>m_enemysToPlayerList;				//壁用のエネミーオブジェクトのリスト
	std::unordered_map<int,Wall*>m_wallList;				//壁のリスト
	CVector3 m_initPlayerPos = { 0.0f,500.0f,0.0f };
	CVector3 m_doorPos = { 0.0f,-350.0f,0.0f };
	//BGM
	CSoundSource m_bgm;
	//
	EnemyType e_type;
};