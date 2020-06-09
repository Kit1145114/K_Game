#pragma once
#include"character/CharacterController.h"
#include"Player.h"
#include"GameSystem/GameConst.h"
#include"sound/SoundEngine.h"
#include"sound/SoundSource.h"

class Player;
class Enemys : public GameObject
{
public:
	 Enemys();
	virtual ~Enemys();
	virtual void Damage(int Damage) = 0;			//DAMAGE
	//エネミーを死亡判定にする。
	virtual void Death()
	{
		isDeath = true;
	}
	//ステートの純粋仮想関数
	virtual void EnemyState() = 0;
	//ここから先は敵の共通する処理を記載。呼び出しはEnemys::〇〇;
	void Draw();				//敵の描画処理。
	void ViewingAngle();		//エネミーの視野角。
	void VectorAcquisition();	//エネミーのベクトルを取得するための関数。
	void Rotation();			//エネミーの回転処理。

public:
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
		esDeath,	//You Are Dead。
		esAttackMe	//攻撃受けた。
	};
	/// <summary>
	/// ボス用の状態。
	/// </summary>
	enum BossAnimState
	{
		bsIdle,			//待機。
		bsWalkTracking,	//歩いて追いかける。
		bsFlyTracking,	//飛行で追いかける。
		bsDeath,		//You Are Dead。
		bsSmallAttack,	//弱攻撃。
		bsBigAttack,	//強攻撃。
		bsHitMe		//攻撃を受けた
	};
	/// <summary>
	/// エネミーに場所を渡す。
	/// </summary>
	/// <param name="position">ポジション</param>
	void SetPosition(CVector3 position)
	{
		m_position = position;
		m_charaCon.SetPosition(m_position);
	}
	/// <summary>
	/// キャラコンに送る。
	/// </summary>
	void SetChaPos(CVector3 pos)
	{
		m_charaCon.SetPosition(pos);
	}
	/// <summary>
	/// 回転くーるくる
	/// </summary>
	void SetRotation(CQuaternion rotation)
	{
		m_rotation = rotation;
	}
	void SetScale(CVector3 scale)
	{
		m_scale = scale;
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
	void SetHitMe(bool isHit)
	{
		isHitMe = isHit;
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
	float walkingDistance = 450.0f;						//歩行距離内。
	float flyDistance = 500.0f;							//飛行距離内
	float m_angle = 0.0f;
	float m_enemytrack = 600.0f;						//追いかける範囲。
	float attackDistance = 150.0f;						//範囲内で攻撃するための変数
	float m_Kyori = 500.0f;
	bool isDeath = false;								//エネミーが死んだかどうか。
	bool isHitMe = false;								//攻撃受けた。
	bool isTracking = false;							//エネミーが追いかけるよ。
	CVector3 m_position= CVector3::Zero();				//エネミーのポジション用のメンバ変数
	CVector3 m_moveSpeed = CVector3::Zero();			//エネミーの移動用のメンバ変数
	CVector3 m_scale = CVector3::One();				//エネミーの大きさ用のメンバ変数。
	CVector3 m_toPlayer = CVector3::Zero();				//プレイヤーに向かうベクトル用。
	CVector3 m_forward = CVector3::AxisZ();				//<エネミーの前方方向。
	CVector3 m_diff = CVector3::Zero();
	CVector3 Move = CVector3::Zero();					//敵が動くよ。（敵機の子）
	CQuaternion m_rotation = CQuaternion::Identity();	//回転用のメンバ変数。
	CharacterController m_charaCon;						//キャラコン。
	EnemyAnimState	e_state;							//エネミーの状態			
	EnemyInitParam prm;									//エネミーのパラメーターが入ってるよ。
	Animation anim;										//アニメ
	AnimationClip animClip[m_AnimClipNum];				//アニメクリップ
	//音
	CSoundSource m_se[3];
};

