#pragma once
#include"Enemys.h"
#include"Object/ITEMBox.h"
#include"character/CharacterController.h"
#include"sound/SoundEngine.h"
#include"sound/SoundSource.h"
#include"graphics/2Dgraphics/Font.h"
#include"physics/PhysicsGhostObject.h"

class Enemys;
class ITEMBox;
class GameCamera;
class IPlayerEventListener;

/// <summary>
/// プレイヤーを制御するクラス
/// </summary>
/// <remark>
/// このクラスはObserverパターンのSubject役のクラスになっています。
/// プレイヤーの状態を監視するObserver役のインスタンスを登録するには
/// AddEventListener関数を使用してください。
/// </remark>
class Player : public GameObject
{					
public:
	/// <summary>
	/// プレイヤーのイベント。
	/// </summary>
	enum Event {
		event_dead,		//死亡した。
		event_num,		//イベントの数。
	};
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
	void Draw();					//プレイヤーの描画処理。
	void FontRender() override;		//文字表示
	void Move();					//プレイヤーの移動処理。
	void MoveOperation();			//プレイヤーの操作。
	void Rotation();				//プレイヤーの回転処理。
	void PlayerState();				//プレイヤーの状態
	void PlayerAttack();			//プレイヤーの攻撃類
	void Energy();					//エネルギー用の関数。
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);		//アニメーションイベント。
	/// <summary>
	/// イベントリスナーの登録。
	/// </summary>
	/// <param name="listener">リスナー。</param>
	void AddEventListener(IPlayerEventListener* listener)
	{
		m_eventListenerList.push_back(listener);
	}
	/// <summary>
	/// イベントリスナーの解除。
	/// </summary>
	/// <param name="listener">リスナー</param>
	void DeleteEvenetListener(IPlayerEventListener* listener)
	{
		auto it = std::find(
			m_eventListenerList.begin(),
			m_eventListenerList.end(),
			listener);
		if (it != m_eventListenerList.end()) {
			//見つかった。
			m_eventListenerList.erase(it);
		}
	}
	/// <summary>
	/// プレイヤーに与えるダメージの設定。
	/// </summary>
	/// <param name="Damage">ダメージ（敵の攻撃力）</param>
	void Damage(int Damage);
	/// <summary>
	/// プレイヤーが死んでいるかどうか。
	/// </summary>
	/// <return>死んだかどうかのフラグ<return>
	bool GetIsDead() const
	{
		return m_isdeath_flag;
	}
	/// <summary>
	/// プレイヤーのポジションを渡す。
	/// </summary>
	/// <param name="m_position">プレイヤーのポジション</param>
	const CVector3& GetPosition()	
	{
		return m_position;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const CVector3& GetRookEnemyPos()
	{
		return m_enemyPos;
	}
	/// <summary>
	/// エネミーのリストを設定する。
	/// </summary>
	/// <param name="GetEnemy">受け取るリスト</param>
	/// <returns></returns>
	const void SetEnemysList(std::vector<Enemys*>& GetEnemy)
	{
		m_enemysList = GetEnemy;
	}
	/// <summary>
	/// ボックスのインスタンス
	/// </summary>
	/// <param name="box"></param>
	void SetBox(ITEMBox* box)
	{
		ItemBox = box;
	}
	/// <summary>
	/// プレイヤーのポジションを受け取る。
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	const void SetPosition(CVector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// プレイヤーの向きを取得。
	/// </summary>
	/// <param name="rot"></param>
	/// <returns></returns>
	const void SetRotation(CQuaternion& rot)
	{
		m_rotation = rot;
	}
	const void SetScale(CVector3& scale)
	{
		m_scale = scale;
	}
	const void SetHP(float& hp) 
	{
		HP = hp;
	}
	//HPバー用の残HPを渡す関数。
	float GetPlayerHP() const
	{
		return HP;
	}
	//エナジー用の残エナジーを渡す関数。
	float GetPlayerEnergy() const
	{
		return ENERGY;
	}
	bool GetIsRooking() const
	{
		return m_isLockOn_flag;
	}
	float GetRadius() 
	{
		return m_charaCon.GetRadius();
	}
	//コンボ関連をリセット
	void ComboReset()
	{
		playerState = pl_idle;
		m_isCombo_flag = false;
		m_ComboNow_flag = false;
	}
	//音
	const void SoundFalse(int SoundNum)
	{
		m_se[SoundNum].Play(false);
	}
	const void SoundTrue(int SoundNum)
	{
		m_se[SoundNum].Play(true);
	}
	const void StopSound(int SoundNum)
	{
		m_se[SoundNum].Stop();
	}
	CVector3 GetForwad() const
	{
		return m_forward;
	}
	CharacterController GetCharaCon()
	{
		return m_charaCon;
	}
	void SetCamera(GameCamera* camera)
	{
		g_Camera = camera;
	}
	//プレイヤーの前ベクトルを取得。
	void Forward();
	//敵をロックオンするときの処理。
	void RookOnEnemys();
	//プレイヤーがコンボ攻撃をする処理。
	void ComboAttack();
	//プレイヤーのHP。
	void PlayerHeal(int healPoint);
	//敵の近くで攻撃したとき、その方向へ向く。
	void toEnemyInduction();
	//計算しましゅ。
	void Angle();
private:
	/// <summary>
	/// 死亡イベントをリスナーに通知する。
	/// </summary>
	void NotifyDeadEventToListener();
	/// <summary>
	/// ロックオン状態が切り替わったイベントをリスナーに通知できるか試す。
	/// </summary>
	/// <param name="isOldLockOnFlag">1フレーム前のロックオンフラグ。</param>
	void TryNotifyChangeLockonEventToListener(bool isOldLockOnFlag);
private:
	SkinModel Gmodel;									//スキンモデル。
	Animation g_anim;									//アニメーション。
	ITEMBox* ItemBox = nullptr;							//アイテム用のもの
	Enemys* enemys;										//敵の情報を得るためのclass的なもの。
	GameCamera* g_Camera;								//カメラ
	std::vector<Enemys*> m_enemysList;					//エネミーのVector配列
	AnimationClip g_animClip[m_AnimClipNum];			//プレイヤーのアニメーションクリップ
	//音
	CSoundSource m_se[4];								//SE
	CVector3 m_position = CVector3::Zero();				//プレイヤーのポジション。	
	CVector3 m_enemyPos = CVector3::Zero();				//エネミーのポジション取得用。
	CVector3 m_moveSpeed = CVector3::Zero();			//プレイヤーの移動用の変数
	CVector3 m_scale = CVector3::One();					//プレイヤーの大きさ用の変数
	CVector3 box_scale = { 125.0f,500.0f,125.0f };		//ボックス大きさ
	CVector3 boxPos;									//ゴーストオブジェクト用のもの
	CVector3 enemy_dis;									//エネミーとの距離。
	CVector3 diff = { 0.0f,0.0f,0.0f };					//エネミーとプレイヤーの距離
	CQuaternion m_rotation = CQuaternion::Identity();	//プレイヤーの軸回転用の変数
	PlayerStateClip playerState;						//プレイヤーのステート
	ENERGYState	playerENE;								//プレイヤーのエナジー。
	CharacterController m_charaCon;						//キャラクターコントローラー
	PhysicsGhostObject m_PhyGhostObj;					//ゴースト
	CVector3 m_forward = CVector3::AxisZ();				//プレイヤーの前方方向。
	CMatrix Rot = CMatrix::Identity();					//プレイヤーの
	CVector3 toPlayer = CVector3::Zero();				//前方向を図るようのもの。
	Font m_font;										//名前。
	int HP = 0;									//プレイヤーのHP
	float MaxHp = 0;										//プレイヤーの最大HP
	int ATK = 0;										//プレイヤーの攻撃力
	int DEF = 0;										//プレイヤーの防御力
	float ENERGY = 0.0f;										//プレイヤーのブースト容量。
	float ENERGYFLUCT = 5.0f;								//エナジー変動値。
	float MAXENERGY = 300.0f;								//最大エナジー。
	float MINENERGY = 0.0f;									//最小エネルギー。
	float None = 0.0f;									//0を代入したいときに使います。
	float m_Rot = 0.0f;									//プレイヤーの回転
	float NSpeed = 1.0f;								//通常のスピード。
	float RunSPeed = 3.5f;								//走れのスピード。
	float JumpPower = 5.0f;								//ジャンプしたときの値
	float m_angle;										//追従用の変数。
	float AttackMoveSpeed = 500.0f;						//攻撃する時の動く値
	float RookAngle = 3.5f;								//敵に向く角度
	float AttackMoveRot = 10.0f;						//攻撃するときに動く角度。
	bool m_isdeath_flag = false;								//死亡判定。
	bool m_isLockOn_flag = false;							//敵をロックオンしているかどうか。
	bool m_isCombo_flag = false;								//コンボするか？
	bool m_ComboNow_flag = false;
	//エフェクト
	Effekseer::Effect* m_attackEffect[3] = { nullptr };
	Effekseer::Handle m_playEffectHandle = -1;
	//追従用の変数
	CVector3 toEnemy = CVector3::Zero();
	CVector3 toNearEnemyPos = CVector3::Zero();
	CVector3 minPos = CVector3::Zero();
	float PlayerRot = 0.0f;
	float NearEnemys = 1000.0f;
	bool isDiscoveryFlag = true;
	//カメラの前方方向と右方向を取得用。
	CVector3 cameraForward;
	CVector3 cameraRight;
	std::list< IPlayerEventListener*> m_eventListenerList;	//イベントリスナーのリスト。
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