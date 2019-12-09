#pragma once
#include "character/CharacterController.h"
#include"GameObjectManajer.h"
#include"physics/PhysicsGhostObject.h"
#include"Enemys.h"

class Enemys;
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
	void Track();			//エネミー探索。
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);		//アニメーションイベント。
	/// <summary>
	/// プレイヤーに与えるダメージの設定。
	/// </summary>
	/// <param name="Damage">ダメージ（敵の攻撃力）</param>
	void SetDamage(float Damage)
	{
		HP -= Damage;
	}	
	/// <summary>
	/// プレイヤーのポジションを渡す。
	/// </summary>
	/// <param name="m_position">プレイヤーのポジション</param>
	CVector3 GetPosition()	const
	{
		return m_position;
	}
	/// <summary>
	/// エネミーを探す。(仮)
	/// </summary>
	void SetEnemys(Enemys * enemy)
	{
		enemys = enemy;
	}
	//プレイヤーの前ベクトルを取得。
	void Forward();
private:
	SkinModel Gmodel;									//スキンモデル。
	Animation g_anim;
	Enemys* enemys;
	//プレイヤーのアニメーション
	AnimationClip g_animClip[3];						//プレイヤーのアニメーションクリップ
	CVector3 m_position = CVector3::Zero();				//プレイヤーのポジション。				
	CVector3 m_moveSpeed = CVector3::Zero();			//プレイヤーの移動用の変数
	CVector3 m_scale = {3.0f,3.0f,3.0f};				//プレイヤーの大きさ用の変数
	CQuaternion m_rotation = CQuaternion::Identity();	//プレイヤーの軸回転用の変数
	PlayerAnimClip plClip;								//プレイヤーのアニメーションステート
	CharacterController m_charaCon;						//キャラクターコントローラー
	PhysicsGhostObject m_PhyGhostObj;					//ゴースト
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
	CVector3 m_forward = CVector3::AxisZ();			//!<エネミーの前方方向。
	CMatrix Rot = CMatrix::Identity();
	CVector3 toPlayer = CVector3::Zero();
	int timer = 0;
	bool atkAction = false;
	float Hasiru = 1.0f;		//走った時に値変更で、アニメーションの速さ変更。
	const wchar_t* attack;
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