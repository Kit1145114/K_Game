#pragma once
#include"graphics/RenderTarget.h"
#include"graphics/c3dmodel/C3DModelDraw.h"

class Sky : public GameObject
{
public:
	Sky();
	~Sky();
	bool Start()override;
	void Update()override;
	void Draw();
public:
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
private:
	SkinModel	SkyModel;						//マップのスキンモデル。
	C3DModelDraw m_mapDraw;					//C3Dモデルドロー。
	CVector3 m_position = CVector3::Zero();	//ポジション
	CVector3 m_scale = {1.4f,1.4f,1.4f};//CVector3::One();		//大きさ
};

