#pragma once
#include"GameObjectManajer.h"
#include"physics/PhysicsStaticObject.h"
#include"RenderTarget.h"
#include"graphics/c3dmodel/C3DModelDraw.h"


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
	SkinModel Map;							//マップのスキンモデル。
	C3DModelDraw m_mapDraw;					//C3Dモデルドロー。
	PhysicsStaticObject m_physicsStaticObj;	//静的オブジェクト
	CVector3 position = CVector3::Zero();	//ポジション
};

