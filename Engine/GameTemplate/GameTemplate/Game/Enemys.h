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
	virtual void Damage(int Damage) = 0;		//DAMAGE
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
	/// <summary>
	/// ボス用の状態。
	/// </summary>
	enum BossAnimState
	{
		bsIdle,			//待機。
		bsTracking,		//追いかける。
		bsDeath,		//You Are Dead。
		bsSmallAttack,	//弱攻撃。
		bsBigAttack		//強攻撃。
	};
	/// <summary>
	/// エネミーに場所を渡す。
	/// </summary>
	/// <param name="position">ポジション</param>
	void SetPosition(CVector3 position)
	{
		m_position = position;
	}
	/// <summary>
	/// 回転くーるくる
	/// </summary>
	void SetRotation(CQuaternion rotation)
	{
		m_rotation = rotation;
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
	CharacterController& GetCharaCon()
	{
		return m_charaCon;
	}
	/// <summary>
	/// エネミーを創るときにパラメーターを入力させる。
	/// </summary>
	/// <param name="HP">ヒットポイント</param>
	/// <param name="Attack">攻撃力</param>
	/// <param name="Defense">防御力</param>
	/// <param name="Speed">基礎速度</param>
	/// <param name="model">エネミーのモデル</param>
	void Init(float HP,float Attack, float Defense,float Speed);
protected:
	Player* m_player;									//プレイヤークラス。
	SkinModel Model;									//エネミーのモデル。
	int m_HP = 0;										//エネミーのHP
	int m_MaxHP = 0;									//エネミーの最大HP
	int m_ATK = 0;										//エネミーの攻撃力
	int m_DEF = 0;										//エネミーの防御力
	int m_SPD = 0;										//エネミーのスピード
	bool isDeath = false;								//エネミーが死んだかどうか。
	CVector3 m_position = CVector3::Zero();				//エネミーのポジション用のメンバ変数
	CVector3 m_moveSpeed = CVector3::Zero();			//エネミーの移動用のメンバ変数
	CVector3 m_scale = CVector3::Zero();				//エネミーの大きさ用のメンバ変数。
	CVector3 m_toPlayer = CVector3::Zero();				//プレイヤーに向かうベクトル用。
	CVector3 m_forward = CVector3::AxisZ();				//<エネミーの前方方向。
	CVector3 Move = CVector3::Zero();					//敵が動くよ。（敵機の子）
	CQuaternion m_rotation = CQuaternion::Identity();	//回転用のメンバ変数。
	CharacterController m_charaCon;						//キャラコン。
	EnemyAnimState	e_state;							//エネミーの状態			
	EnemyInitParam prm;									//エネミーのパラメーターが入ってるよ。
	Animation anim;										//アニメ
	AnimationClip animClip[m_AnimClipNum];				//アニメクリップ
};

