#pragma once

class Player;
class Door : public GameObject
{
public:
	Door();
	~Door();
	void Update();					//更新処理。
	void Draw();					//描画処理。
	void PlayerDistance();			//プレイヤーが接近して扉に入る処理。

public:
	//外部からpositionを設定。
	const void SetPosition(CVector3& pos)
	{
		m_position = pos;
	}
	void SetPlayer(Player* pl)
	{
		m_player = pl;
	};

	const CVector3& GetPosition()
	{
		return m_position;
	}
	const bool& GetChangeSta()
	{
		return isChangeStage_flag;
	}
private:
	Player* m_player;
	SkinModel Gate;							//マップのスキンモデル。
	CVector3 m_position = CVector3::Zero();			//ポジション
	CVector3 m_scale = CVector3::One();			//スケール。
	CQuaternion m_rotation = CQuaternion::Identity();	//ローテーション。
	CVector3 m_distance = CVector3::Zero();
	float m_warpDistance = 125.0f;
	bool isChangeStage_flag = false;
};

