#pragma once
#include"GameObjectManajer.h"
#include"graphics/c3dmodel/C3DModelDraw.h"
#include"RenderTarget.h"

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
private:
	Player* player;					//プレイヤーのインスタンス。
	MAP* map;						//マップのインスタンス。
	GameCamera* g_Camera;			//カメラのインスタンス。
	Enemys* enemys[4];				//エネミーのインスタンス。
	HPText* hp_bar;					//HPバー。
	AgoSample* ago;					//顎。
	static Game* m_instance;		//ゲームのインスタンス。
	const int ENEMY_NUM = 4;		//登場するエネミーの数をここで定義。

	//ここからレンダリングターゲット関係のメンバ変数です。

	C3DModelDraw m_playerDraw;
	C3DModelDraw m_mapDraw;
	C3DModelDraw m_enemysDraw[4];
	C3DModelDraw m_AgoDraw;
};

