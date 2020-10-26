#pragma once
#include"Chara/IPlayerEventListener.h"

class Player;
/// <summary>
/// ゲームカメラクラス
/// </summary>
/// <remark>
/// このクラスはObserverパターンのConcreteObserver役のクラスです。
/// このクラスはプレイヤーの状態を監視しています。
/// </remark>
class GameCamera : public GameObject ,IPlayerEventListener
{
public:
	/// <summary>
	/// カメラのステート
	/// </summary>
	enum CameraState {
		toPlayer,
		toEnemys
	};

	/// <summary>
	/// インストラクタ
	/// </summar>
	GameCamera();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameCamera();
	bool Start()override;			//プレイヤーのイベントリスナー登録がメイン。
	void Update() override;			//カメラの更新関数。
	void State();					//カメラの状態。
	/// <summary>
	/// プレイヤーの座標を持ってくる。
	/// </summary>
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	//カメライェア
	void PlayerDamageRot();
	/// <summary>
	/// カメラの角度変更
	/// </summary>
	void CameraRotate();
	/// <summary>
	/// カメラを敵にロックする。
	/// </summary>
	void CameraLookEnemys();
	/// <summary>
	/// プレイヤーがロックオンしている時常に呼ばれる処理。
	/// </summary>
	void OnStartLockOn(Player* pl)override;
	/// <summary>
	/// プレイヤーがロックオンしていない時に常に呼ばれる処理。
	/// </summary>
	void OnEndLockOn(Player* pl)override;
	/// <summary>
	/// プレイヤーがダメージを受けたときに画面が揺れる処理。
	/// </summary>
	void OnDamage(Player* pl)override;

	CVector3 GetPosition() const
	{
		return m_position;
	}
private:
	Player* m_player = nullptr;							//プレイヤークラスの初期化。
	CVector3 m_position = CVector3::Zero();				//カメラの座標
	CVector3 m_playerPosition = CVector3::Zero();		//円の中心座標(プレイヤーの座標)
	CVector3 m_enemyPosition = CVector3::Zero();		//ロックオンするエネミーの座標。
	CVector3 m_target = CVector3::Zero();				//カメラの注視点
	CQuaternion m_rotate = CQuaternion::Identity();		//カメラの回転。
	CameraState c_state;								//カメラのステート。
	CVector3 m_moveCamera = { 5.0f,0.0f,0.0f };
	//カメラの座標および注視点の計算用。
	void Hutu();
	CVector3 m_toPos = { 0.0f, 0.0f, 0.0f };
	//角度
	float m_degreexz = 0.0f;
	float m_degreey = 0.0f;
	//回転速度
	float m_sdegreexz = 0.0f;
	float m_sdegreey = 0.0f;
	//保存角度
	float m_memoryDegreexz = 0.0f;
	float m_moveNum = 4.0f;
	bool damage_flag = false;
	float timer = 0.0f;
	//半径
	const float m_r = 250.0f;
	//角度をラジアン単位(π)に直したやつ
	double m_radianx;
	double m_radiany;
	//
	CVector3 m_right = CVector3::Zero();
};