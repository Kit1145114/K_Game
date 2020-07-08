#pragma once
#include"character/CharacterController.h"
#include"physics/PhysicsStaticObject.h"

class PhysicsStaticObject;
class Wall : public GameObject
{
public:

	enum WallState
	{
		w_idle,
		w_move,
		m_death
	};

	Wall();
	~Wall();
	bool Start() override;
	void Update() override;
	void Draw();
	void State();
	void Death();
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	void SetRotation(CQuaternion rot)
	{
		m_rotation = rot;
	}
	void SetObjNum(int num)
	{
		m_objNum = num;
	}
	int GetObjData()
	{
		return m_objNum;
	}
private:
	SkinModel  m_wall;
	CharacterController m_charaCon;						//キャラコン。
	CVector3 m_position = CVector3::Zero();				//ポジション
	CVector3 m_scale = CVector3::One();				//スケール。
	CQuaternion m_rotation = CQuaternion::Identity();	//ローテーション。
	PhysicsStaticObject m_physicsStaticObj;	//静的オブジェクト
	WallState wallState;
	int m_objNum = 0;
	bool m_deathFlag = false;
};

