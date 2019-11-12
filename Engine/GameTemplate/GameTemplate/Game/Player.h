#pragma once
#include "character/CharacterController.h"
#include"GameObjectManajer.h"

class Player : public GameObject
{					
public:
	/// <summary>
	/// プレイヤーのアニメーション用のステート
	/// </summary>
	enum PlayerAnimClip 
	{
		plAnimClip_idle,		//待機
		plAnimClip_Walk,		//歩き
		plAnimClip_Atk			//攻撃
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
	void SetDamage(float Damage)
	{
		HP -= Damage;
	}
	CVector3 GetPosition()
	{
		return m_position;
	}
	//static Player*GetPlayer();
private:
	SkinModel Gmodel;									//スキンモデル。
	Animation g_anim;									//プレイヤーのアニメーション
	AnimationClip g_animClip[3];						//プレイヤーのアニメーションクリップ
	CVector3 m_position = CVector3::Zero();				//プレイヤーのポジション。				
	CVector3 m_moveSpeed = CVector3::Zero();			//プレイヤーの移動用の変数
	CVector3 m_scale = CVector3::Zero();				//プレイヤーの大きさ用の変数
	CQuaternion m_rotation = CQuaternion::Identity();	//プレイヤーの軸回転用の変数
	PlayerAnimClip plClip;								//プレイヤーのステート変数
	//bool PlayerMove = false;
	//プレイヤーの移動量。
	CharacterController m_charaCon;		//キャラクターコントローラー
	static Player* player;
	/// <summary>
	/// プレイヤーのHP
	/// </summary>
	float HP = 0;
	/// <summary>
	/// プレイヤーの攻撃力
	/// </summary>
	float ATK = 0;
	/// <summary>
	/// プレイヤーの防御力
	/// </summary>
	float DEF = 0;
	int timer = 0;
	bool atkAction = false;
	float Hasiru = 1.0f;		//走った時に値変更で、アニメーションの速さ変更。
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