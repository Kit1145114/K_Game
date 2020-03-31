#pragma once
#include"physics/PhysicsStaticObject.h"
#include"RenderTarget.h"
#include"graphics/c3dmodel/C3DModelDraw.h"
#include"level/Level.h"

class Enemys;
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
	void SetPosition(CVector3 pos)
	{
		position = pos;
	}
private:
	SkinModel	Map;						//マップのスキンモデル。
	Level		mapLevel;					//マップのレベル。。。。。。
	std::vector<Enemys*> m_enemysList;		//エネミーオブジェクトのリスト
	C3DModelDraw m_mapDraw;					//C3Dモデルドロー。
	PhysicsStaticObject m_physicsStaticObj;	//静的オブジェクト
	CVector3 position = CVector3::Zero();	//ポジション
};

