#pragma once

class Player;
class Title;
class Door : public GameObject
{
public:
	Door();
	~Door();
	void Update();
	void Draw();
	void PlayerDistance();

public:
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	void SetPlayer(Player* pl)
	{
		m_player = pl;
	};

	CVector3& GetPosition()
	{
		return m_position;
	}
	bool GetChangeSta()
	{
		return ChangeStage;
	}
private:
	Player* m_player;
	Title* title;
	SkinModel Gate;							//マップのスキンモデル。
	CVector3 m_position = CVector3::Zero();			//ポジション
	CVector3 m_scale = CVector3::One();			//スケール。
	CQuaternion m_rotation = CQuaternion::Identity();	//ローテーション。
	CVector3 m_distance = CVector3::Zero();
	float m_warpDistance = 125.0f;
	bool ChangeStage = false;
};

