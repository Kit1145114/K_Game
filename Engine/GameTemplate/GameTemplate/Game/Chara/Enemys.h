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
		isDeath_flag = true;
	}
	//ステートの純粋仮想関数
	virtual void EnemyState() = 0;
	virtual void Draw() override final; 				//敵の描画処理。
	//ここから先は敵の共通する処理を記載。呼び出しはEnemys::〇〇;

	void ViewingAngle();		//エネミーの視野角。
	void VectorAcquisition();	//エネミーのベクトルを取得するための関数。
	void Rotation();			//エネミーの回転処理。
	void EnemyEffect();			//エネミーのエフェクト。
public:
	//エネミーが死んだかどうかを返す。
	bool GetisDeath() const
	{
		return isDeath_flag;
	}
	struct EnemyInitParam
	{
		int HP;
		int ATK;
		int DEF;
		float SPD;
		//SkinModel model;
	};
	/// <summary>
	/// 敵の状態。
	/// </summary>
	enum EnemyAnimState
	{
		esIdle,			//待機。
		esTracking,		//追いかける。
		esAttack,		//攻撃。
		esDeath,		//You Are Dead。
		esAttackMe,		//攻撃受けた。
		esAttackGap,	//攻撃の隙
		esStandbyAttack	//攻撃に入るまで
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
		bsHitMe,		//攻撃を受けた
	};
	/// <summary>
	/// エネミーに場所を渡す。
	/// </summary>
	/// <param name="position">ポジション</param>
	void SetPosition(const CVector3& position)
	{
		m_position = position;
		m_charaCon.SetPosition(m_position);
	}
	/// <summary>
	/// キャラコンに送る。
	/// </summary>
	void SetChaPos(const CVector3& pos)
	{
		m_charaCon.SetPosition(pos);
	}
	/// <summary>
	/// 回転くーるくる
	/// </summary>
	void SetRotation(const CQuaternion& rotation)
	{
		m_rotation = rotation;
	}
	void SetScale(const CVector3& scale) 
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
	bool GetIsDead() const
	{
		return isDeath_flag;
	}
	CharacterController& GetCharaCon() 
	{
		return m_charaCon;
	}
	void SetHitMe(bool isHit)
	{
		isHitMe_flag = isHit;
	}
	void SetObjNum(int num)
	{
		m_objNum = num;
	}
	int GetObjData() const
	{
		return m_objNum;
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
	Player* m_player = nullptr;									//プレイヤークラス。
	SkinModel Model;									//エネミーのモデル。
	float m_HP = 0.0f;										//エネミーのHP
	float m_MaxHP = 0.0f;									//エネミーの最大HP
	int m_ATK = 0;										//エネミーの攻撃力
	int m_DEF = 0;										//エネミーの防御力
	int m_SPD = 0;										//エネミーの初期スピード。
	int m_objNum = 0;
	//エネミーのスピード
	float m_walkingDistance = 300.0f;						//歩行距離内。
	float m_flyDistance = 500.0f;							//飛行距離内
	float m_angle = 0.0f;
	float m_enemyTrack = 1250.0f;						//追いかける範囲。
	float m_attackDistance = 125.0f;					//範囲内で攻撃するための変数
	float m_Kyori = 500.0f;								
	float m_timer = 0.0f;								//タイマー。
	float m_fowndAngle = 0.4f;							//見つける角度
	bool isDeath_flag = false;							//エネミーが死んだかどうか。
	bool isHitMe_flag = false;							//攻撃受けた。
	bool isAttack_flag = false;							//エネミーが追いかけるよ。
	bool isTrack_flag = false;							//こちらは別の追いかけるフラグ。
	bool isDestination_flag = true;						//一回だけ目的地を決めたいので。
	bool isEffectLoop_flag = true;			//エフェクトを大量生成しないように使うフラグ。
	CVector3 m_position= CVector3::Zero();				//エネミーのポジション用のメンバ変数
	CVector3 m_moveSpeed = CVector3::Zero();			//エネミーの移動のスピードだよ。
	CVector3 m_scale = CVector3::One();					//エネミーの大きさ用のメンバ変数。
	CVector3 m_toPlayer = CVector3::Zero();				//プレイヤーに向かうベクトル用。
	CVector3 m_forward = CVector3::AxisZ();				//<エネミーの前方方向。
	CVector3 m_diff = CVector3::Zero();
	CVector3 m_move = CVector3::Zero();					//敵が動くよ。（敵機の子）
	CVector3 m_destination = CVector3::Zero();			//動く場所まで。
	CVector3 m_efePos;									//エフェクトのポジション
	CVector3 m_efeRot;									//エフェクトの回転
	CQuaternion m_rotation = CQuaternion::Identity();	//回転用のメンバ変数。
	CharacterController m_charaCon;						//キャラコン。
	EnemyAnimState	e_state;							//エネミーの状態。			
	EnemyInitParam prm;									//エネミーのパラメーターが入ってるよ。
	Animation anim;										//アニメ。
	AnimationClip animClip[m_AnimClipNum];				//アニメクリップ。
	//音
	CSoundSource m_se[10];								//音。
	//エフェクト
	Effekseer::Effect* m_attackEffect = nullptr;		//基本攻撃中のエフェクト。
	Effekseer::Handle m_playEffectHandle = 1;
};

