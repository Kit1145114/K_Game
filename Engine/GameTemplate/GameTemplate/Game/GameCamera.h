#pragma once
#include"GameObjectManajer.h"
#include"Enemys.h"

class Player;
class Enemys;
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
	void Update();		//カメラの更新関数。
	/// <summary>
	/// プレイヤーのざひょうもってくりゅ。
	/// </summary>
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	/// <summary>
	/// ロックする敵の情報をもってくる。
	/// </summary>
	/// <param name="GetEnemy"></param>
	void SetEnemysList(std::vector<Enemys*> GetEnemy)
	{
		m_goList = GetEnemy;
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
	//CVector3型のポジションの情報を扱うメンバ変数
	CVector3 m_position = CVector3::Zero();	//初期位置（0）を代入。
	CQuaternion m_rotate = CQuaternion::Identity();
	Player* m_player = nullptr;
	Enemys* enemys;										//敵の情報を得るためのclass的なもの。s
	std::vector<Enemys*> m_goList;						//ゲームオブジェクトのリスト
	CameraState c_State;								//カメラのステート。
	//カメラの座標および注視点の計算
	void Hutu();
	CVector3 m_toPos = { 0.0f, 0.0f, 0.0f };
	//円の中心座標(プレイヤーの座標)
	CVector3 m_playerposition;
	//角度
	float m_degreexz = 0.0f;
	float m_degreey = 0.0f;
	//回転速度
	float m_sdegreexz = 0.0f;
	float m_sdegreey = 0.0f;
	//半径
	const float m_r = 170.0f;
	//角度をラジアン単位(π)に直したやつ
	double m_radianx;
	double m_radiany;
	//注視点
	CVector3 m_target;
	//カメラの座標
	//CVector3 m_position;
};