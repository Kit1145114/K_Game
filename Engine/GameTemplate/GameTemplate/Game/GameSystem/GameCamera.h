#pragma once

class Player;
class GameCamera : public GameObject
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
	void Update() override;			//カメラの更新関数。
	void State();			//カメラの状態。
	/// <summary>
	/// プレイヤーのざひょうもってくりゅ。
	/// </summary>
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	/// <summary>
	/// カメラの角度変更
	/// </summary>
	void CameraRotate();
	/// <summary>
	/// カメラを敵にロックする。
	/// </summary>
	void CameraLookEnemys();
private:
	Player* m_player = nullptr;							//プレイヤークラスの初期化。
	CVector3 m_position = CVector3::Zero();				//カメラの座標
	CVector3 m_playerposition = CVector3::Zero();		//円の中心座標(プレイヤーの座標)
	CVector3 m_enemyposition = CVector3::Zero();		//ロックオンするエネミーの座標。
	CVector3 m_target = CVector3::Zero();				//カメラの注視点
	CQuaternion m_rotate = CQuaternion::Identity();		//カメラの回転。
	CameraState c_State;								//カメラのステート。

	//カメラの座標および注視点の計算用。
	void Hutu();
	CVector3 m_toPos = { 0.0f, 0.0f, 0.0f };
	//角度
	float m_degreexz = 0.0f;
	float m_degreey = 0.0f;
	//回転速度
	float m_sdegreexz = 0.0f;
	float m_sdegreey = 0.0f;
	//半径
	const float m_r = 250.0f;
	//角度をラジアン単位(π)に直したやつ
	double m_radianx;
	double m_radiany;
};