#pragma once
#include"GameObjectManajer.h"
#include"character/CharacterController.h"
#include"Player.h"
#include"GameConst.h"

class Enemy;
class Player;

class Enemys : public GameObject
{
public:
	 Enemys();
	virtual ~Enemys();

	//virtual void Attack() = 0;					//攻撃
	virtual void Damage(float Damage) = 0;		//DAMAGE
	//エネミーを死亡判定にする。
	virtual void Death()
	{
		isDeath = true;
	}

	virtual void EnemyState() = 0;

	//エネミーが死んだかどうかを返す。
	bool GetisDeath()
	{
		return isDeath;
	}

	struct EnemyInitParam
	{
		float HP;
		float ATK;
		float DEF;
		float SPD;
		//SkinModel model;
	};
	/// <summary>
	/// 敵の状態。
	/// </summary>
	enum EnemyAnimState
	{
		esIdle,		//待機。
		esTracking,	//追いかける。
		esAttack,	//攻撃。
		esDeath		//You Are Dead。
	};
	void SetEnemyID(int ID)
	{
		m_ID = ID;
	}
	/// <summary>
	/// エネミーに場所を渡す。
	/// </summary>
	/// <param name="position">ポジション</param>
	void SetPosition(CVector3 position)
	{
		m_position = position;
	}
	//ポジションもらえます。
	CVector3 GetPosition()	const
	{
		return m_position;
	}
	//プレイヤー...いただいてます。
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	//死亡かどうか。
	bool GetIsDead()
	{
		return isDeath;
	}
	/// <summary>
	/// エネミーを創るときにパラメーターを入力させる。
	/// </summary>
	/// <param name="HP">ヒットポイント</param>
	/// <param name="Attack">攻撃力</param>
	/// <param name="Defense">防御力</param>
	/// <param name="Speed">基礎速度</param>
	/// <param name="model">エネミーのモデル</param>
	void Init(float HP,float Attack, float Defense,float Speed /*,SkinModel model*/);
	//bool Start();
	//void Update();
	//void Draw();
protected:
	/// <summary>
	/// プレイヤークラス。
	/// </summary>
	Player* m_player;
	/// <summary>
	/// エネミーのモデル。
	/// </summary>
	SkinModel m_sm;
	/// <summary>
	/// エネミーのHP
	/// </summary>
	float m_HP = 0;
	/// <summary>
	/// エネミーの最大HP
	/// </summary>
	float m_MaxHP = 0;
	/// <summary>
	/// エネミーの攻撃力。
	/// </summary>
	float m_ATK = 0;
	/// <summary>
	/// エネミーの防御力。
	/// </summary>
	float m_DEF = 0;
	/// <summary>
	/// エネミーの基礎速度。
	float m_SPD = 0;
	/// </summary>

	int m_ID = 0;
	CVector3 m_position = CVector3::Zero();					/// エネミーのポジション用のメンバ変数
	CVector3 m_moveSpeed = CVector3::Zero();				/// エネミーの移動用のメンバ変数
	CQuaternion m_rotation = CQuaternion::Identity();		//回転用のメンバ変数。
	/// <summary>
	/// エネミーが死んだかどうかの変数
	/// </summary>
	bool isDeath = false;
	CharacterController m_charaCon;							// キャラコン。
	EnemyAnimState	e_state;								//エネミーの状態			
	CVector3 Move = CVector3::Zero();						//敵が動くよ。（敵機の子）
};

