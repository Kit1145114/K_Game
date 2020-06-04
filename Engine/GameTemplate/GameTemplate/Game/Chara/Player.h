#pragma once
#include"Enemys.h"
#include"ITEM/ITEMBox.h"
#include"character/CharacterController.h"
#include"sound/SoundEngine.h"
#include"sound/SoundSource.h"
#include"graphics/2Dgraphics/Font.h"
#include"physics/PhysicsGhostObject.h"

class Enemys;
class ITEMBox;
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
		pl_Combo,		//コンボ中。
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
	void Update()override;			//プレイヤーの更新関数
	void Draw();			//プレイヤーの描画処理。
	void Render()override;	//レンダー
	void FontRender() override;  //文字表示
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
		return m_isdeath;
	}
	/// <summary>
	/// プレイヤーのポジションを渡す。
	/// </summary>
	/// <param name="m_position">プレイヤーのポジション</param>
	const CVector3& GetPosition()	
	{
		return m_position;
	}
	const CVector3& GetRookEnemyPos()
	{
		return m_enemyPos;
	}
	void SetEnemysList(std::vector<Enemys*> GetEnemy)
	{
		m_enemysList = GetEnemy;
	}

	void SetBox(ITEMBox* box)
	{
		ItemBox = box;
	}
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	void SetRotation(CQuaternion rot)
	{
		m_rotation = rot;
	}
	void SetScale(CVector3 scale)
	{
		m_scale = scale;
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
	bool GetIsRooking()
	{
		return m_isRookOn;
	}
	//コンボ関連をリセット
	void ComboReset()
	{
		playerState = pl_idle;
		m_isCombo = false;
		m_ComboNow = false;
	}
	//音
	void Sound(int SoundNum)
	{
		m_se[SoundNum].Play(false);
	}
	void StopSound(int SoundNum)
	{
		m_se[SoundNum].Stop();
	}
	//プレイヤーの前ベクトルを取得。
	void Forward();
	//敵をロックオンするときの処理。
	void RookOnEnemys();
	//プレイヤーがコンボ攻撃をする処理。
	void ComboAttack();
private:
	SkinModel Gmodel;									//スキンモデル。
	Animation g_anim;									//アニメーション。
	ITEMBox* ItemBox = nullptr;									//アイテム用のもの
	Enemys* enemys;										//敵の情報を得るためのclass的なもの。
	std::vector<Enemys*> m_enemysList;					//エネミーのVector配列
	AnimationClip g_animClip[m_AnimClipNum];			//プレイヤーのアニメーションクリップ
	//音
	CSoundSource m_se[2];								//SE
	CVector3 m_position = CVector3::Zero();				//プレイヤーのポジション。	
	CVector3 m_enemyPos = CVector3::Zero();				//エネミーのポジション取得用。
	CVector3 m_moveSpeed = CVector3::Zero();			//プレイヤーの移動用の変数
	CVector3 m_scale = CVector3::One();				//プレイヤーの大きさ用の変数
	CVector3 box_scale = { 50.0f,50.0f,50.0f };			//ボックス大きさ
	CVector3 diff = { 0.0f,0.0f,0.0f };					//エネミーとプレイヤーの距離
	CQuaternion m_rotation = CQuaternion::Identity();	//プレイヤーの軸回転用の変数
	PlayerStateClip playerState;						//プレイヤーのステート
	ENERGYState	playerENE;								//プレイヤーのエナジー。
	CharacterController m_charaCon;						//キャラクターコントローラー
	PhysicsGhostObject m_PhyGhostObj;					//ゴースト
	CVector3 m_forward = CVector3::AxisZ();				//プレイヤーの前方方向。
	CMatrix Rot = CMatrix::Identity();					//プレイヤーの
	CVector3 toPlayer = CVector3::Zero();				//前方向を図るようのもの。
	Font m_font;									//名前。
	int HP = 0;											//プレイヤーのHP
	int ATK = 0;										//プレイヤーの攻撃力
	int DEF = 0;										//プレイヤーの防御力
	int ENERGY = 0;										//プレイヤーのブースト容量。
	int ENERGYFLUCT = 5;								//エナジー変動値。
	int MAXENERGY = 300;								//最大エナジー。
	int MINENERGY = 0;									//最小エネルギー。
	float None = 0.0f;									//0を代入したいときに使います。
	float NSpeed = 1.0f;								//通常のスピード。
	float SPeed2 = 3.5f;								//２倍のスピード。
	float JumpPower = 40.0f;							//ジャンプしたときの値
	bool m_isdeath = false;								//死亡判定。
	bool m_isRookOn = false;							//敵をロックオンしているかどうか。
	bool m_isCombo = false;								//コンボするか？
	bool m_ComboNow = false;
};
/// <summary>
/// プレイヤー、タグ。
/// </summary>
/// <remarks>詳細な情報を書くためのタグ</remarks>
/// <param name="num">動く歩数。</param>
/// <param name="value">一歩の量</param>
/// <returns>実際に動けた歩数</returns>
/// <example>コードの説明</example>
/// <include file='' path='[@name=""]'/>