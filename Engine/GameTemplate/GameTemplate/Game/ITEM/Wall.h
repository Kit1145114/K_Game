#pragma once
#include"character/CharacterController.h"
#include"physics/PhysicsStaticObject.h"
#include"../GameSystem/GameConst.h"

class PhysicsStaticObject;
class Wall : public GameObject
{
public:
	enum WallState
	{
		w_idle,
		w_move,
		w_death
	};
	Wall();
	~Wall();
	bool Start() override;
	void Update() override;
	void Move();
	void Draw();
	void State();
	void Death();
	void Idle();
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
	void SetMoveflag(bool flag)
	{
		m_moveFlag = flag;
	}
	int GetObjData()
	{
		return m_objNum;
	}
private:
	SkinModel  m_wall;
	CharacterController m_charaCon;						//キャラコン。
	CVector3 m_position = CVector3::Zero();				//ポジション
	CVector3 m_scale = CVector3::One();					//スケール。
	CQuaternion m_rotation = CQuaternion::Identity();	//ローテーション。
	PhysicsStaticObject m_physicsStaticObj;				//静的オブジェクト
	Animation w_anim;									//アニメーション。
	AnimationClip w_animClip[m_AnimClipNum];			//プレイヤーのアニメーションクリップ
	WallState wallState;
	int m_objNum = 0;									
	bool m_moveFlag = false;							//動いていいか
	bool m_deathFlag = false;							//死
	float m_timer = 0.0f;								//動く時間
};

