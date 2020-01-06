#pragma once
#include"GameObjectManajer.h"
#include"physics/PhysicsStaticObject.h"
#include"physics/PhysicsGhostObject.h"

class PhysicsGhostObject;

class AgoSample : public GameObject
{
public:
	AgoSample();
	~AgoSample();
	void Update();
	void Draw();
private:
	SkinModel Ago;	//マップのスキンモデル。
	PhysicsGhostObject m_PhyGhostObj;
	//PhysicsStaticObject m_PSO;				//静的オブジェクト
	CVector3 m_position = {0.0f,-250.0f,0.0f};
	CVector3 m_scale = {100.0f,100.0f,100.0f};
	CQuaternion Rot = { 0.0f,0.0f,0.0f,0.0f };
};

