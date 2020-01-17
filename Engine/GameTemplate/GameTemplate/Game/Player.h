#pragma once
#include "character/CharacterController.h"
#include"GameObjectManajer.h"
#include"physics/PhysicsGhostObject.h"
#include"Enemys.h"
#include"GameConst.h"

class Enemys;
class Player : public GameObject
{					
public:
	/// <summary>
	/// プレイヤーのステート
	/// </summary>
	enum PlayerStateClip
	{
		pl_idle,		//待機
		pl_Walk,		//歩き
		pl_FlyMove,		//浮遊移動。
		pl_Atk,			//攻撃
		pl_Death		//死亡
	};
	//プレイヤーのエナジー。
	enum ENERGYState
	{
		ene_Charge,			//charge中
		ene_Full			//動ける
	};
	/// <summary>
	/// インストラクタ
	/// </summary>
	Player();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();
	void Update();			//プレイヤーの更新関数
	void Draw();			//プレイヤーの描画処理。
	void Move();			//プレイヤーの移動処理。
	void MoveOperation();		//プレイヤーの操作。
	void Rotation();		//プレイヤーの回転処理。
	void PlayerState();		//プレイヤーの状態
	void PlayerAttack();	//プレイヤーの攻撃類
	void Track();			//エネミー探索。
	void Energy();			//エネルギー用の関数。
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);		//アニメーションイベント。
	/// <summary>
	/// プレイヤーに与えるダメージの設定。
	/// </summary>
	/// <param name="Damage">ダメージ（敵の攻撃力）</param>
	void Damage(int Damage);
	bool GetIsDead()
	{
		return m_death;
	}
	/// <summary>
	/// プレイヤーのポジションを渡す。
	/// </summary>
	/// <param name="m_position">プレイヤーのポジション</param>
	CVector3 GetPosition()	const
	{
		return m_position;
	}
	void SetEnemysList(std::vector<Enemys*> GetEnemy)
	{
		m_goList = GetEnemy;
	}
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	void SetRotation(CQuaternion rot)
	{
		m_rotation = rot;
	}
	//HPバー用の残HPを渡す関数。
	int GetPlayerHP()
	{
		return HP;
	}
	//エナジー用の残エナジーを渡す関数。
	int GetPlayerEnergy()
	{
		return ENERGY;
	}
	//プレイヤーの前ベクトルを取得。
	void Forward();

private:
	SkinModel Gmodel;									//スキンモデル。
	Animation g_anim;									//アニメーション。
	Enemys* enemys;										//敵の情報を得るためのclass的なもの。s
	std::vector<Enemys*> m_goList;						//ゲームオブジェクトのリスト
	AnimationClip g_animClip[m_AnimClipNum];			//プレイヤーのアニメーションクリップ
	CVector3 m_position = CVector3::Zero();				//プレイヤーのポジション。	
	CVector3 m_moveSpeed = CVector3::Zero();			//プレイヤーの移動用の変数
	CVector3 m_scale = {3.0f,3.0f,3.0f};				//プレイヤーの大きさ用の変数
	CVector3 box_scale = { 50.0f,50.0f,50.0f };			//ボックス大きさ
	CQuaternion m_rotation = CQuaternion::Identity();	//プレイヤーの軸回転用の変数
	PlayerStateClip playerState;						//プレイヤーのステート
	ENERGYState	playerENE;								//プレイヤーのエナジー。
	CharacterController m_charaCon;						//キャラクターコントローラー
	PhysicsGhostObject m_PhyGhostObj;					//ゴースト
	CVector3 m_forward = CVector3::AxisZ();				//プレイヤーの前方方向。
	CMatrix Rot = CMatrix::Identity();					//プレイヤーの
	CVector3 toPlayer = CVector3::Zero();				//前方向を図るようのもの。
	int HP = 0;											//プレイヤーのHP
	int ATK = 0;										//プレイヤーの攻撃力
	int DEF = 0;										//プレイヤーの防御力
	int ENERGY = 0;										//プレイヤーのブースト容量。
	int ENERGYFLUCT = 10;								//エナジー変動値。
	int MAXENERGY = 300;								//最大エナジー。
	int MINENERGY = 0;									//最小エネルギー。
	float None = 0.0f;									//0を代入したいときに使います。
	float NSpeed = 1.0f;								//通常のスピード。
	float SPeed2 = 3.5f;								//２倍のスピード。
	float JumpPower = 40.0f;							//ジャンプしたときの値
	bool m_death = false;								//死亡判定。
};
//	クラス、関数はコーディングルール	アッパーキャメルMoveCount
//	変数は　アンダーキャメル			m_moveCount		
//　メンバ変数にはm_を付ける。
//	統一することが大事。
//引数、変数、戻り値、メンバ変数にコメント
/// <summary>
/// プレイヤー、タグ。
/// </summary>
/// <remarks>詳細な情報を書くためのタグ</remarks>
/// <param name="num">動く歩数。</param>
/// <param name="value">一歩の量</param>
/// <returns>実際に動けた歩数</returns>
/// <example>コードの説明</example>
/// <include file='' path='[@name=""]'/>