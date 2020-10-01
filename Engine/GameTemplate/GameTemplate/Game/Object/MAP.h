#pragma once
#include"physics/PhysicsStaticObject.h"
#include"graphics/RenderTarget.h"
#include"graphics/c3dmodel/C3DModelDraw.h"
#include"level/Level.h"

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
	bool Start()override;
	void Update()override;
	void Draw();
	//外部からポジションを設定。
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	//外部からスケールを設定。
	void SetScale(CVector3 scale)
	{
		m_scale = scale;
	}
	//外部からステージを設定。
	void SetStage(int num)
	{
		m_stage = num;
	}
private:
	SkinModel	Map;						//マップのスキンモデル。
	Level		mapLevel;					//マップのレベル。。。。。。
	C3DModelDraw m_mapDraw;					//C3Dモデルドロー。
	PhysicsStaticObject m_physicsStaticObj;	//静的オブジェクト
	CVector3 m_position = CVector3::Zero();	//ポジション
	CVector3 m_scale = CVector3::One();		//大きさ
	int m_stage = 0;
};

