#pragma once
#include"GameObjectManajer.h"
#include"physics/PhysicsStaticObject.h"

class PhysicsStaticObject;
class MAP	:	public GameObject
{
public:
	/// <summary>
	/// インストラクタ
	/// </summary>
	MAP();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~MAP();
	void Update();
	void Draw();
private:
	SkinModel Map;	//マップのスキンモデル。
	PhysicsStaticObject m_PSO;				//静的オブジェクト
	CVector3 position = CVector3::Zero();
};

