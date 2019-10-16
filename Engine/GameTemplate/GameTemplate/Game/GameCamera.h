#pragma once
#include"GameObjectManajer.h"

class Player;
class GameCamera : public GameObject
{
public:
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
private:
	//CVector3型のポジションの情報を扱うメンバ変数
	CVector3 m_position = CVector3::Zero();	//初期位置（0）を代入。
	Player* m_player = nullptr;
};

