#pragma once
#include"character/CharacterController.h"

class Wall : public GameObject
{
public:
	Wall();
	~Wall();
	bool Start() override;
	void Update() override;
	void Draw();
private:
	SkinModel  m_wall;
	CharacterController m_charaCon;						//キャラコン。
	CVector3 m_position = CVector3::Zero();				//ポジション
	CVector3 m_scale = CVector3::One();				//スケール。
	CQuaternion m_rotation = CQuaternion::Identity();	//ローテーション。
};

