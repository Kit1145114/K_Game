#pragma once
#include"graphics/Sprite.h"
#include"GameObjectManajer.h"

class SpriteRender : public GameObject
{
public:
	SpriteRender();
	~SpriteRender(); 
	void Init(const wchar_t* texFilePath, float w, float h, bool isDraw3D = false);
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	void SetPivot(const CVector2& pivot)
	{
		m_pivot = pivot;
	}
	void Update();
	void Draw();
private:
	bool m_isDraw3D = false;
	Sprite	m_sprite;				//!<スプライト。
	CVector3 m_position = CVector3::Zero();
	CQuaternion m_rotation = {0.0f,1.0f,0.0f,0.0f};			//!<回転。
	CVector3 m_scale = CVector3::One();			//!<拡大率。
	CVector2 m_pivot = { 0.5f,0.5f };//Sprite::DEFAULT_PIVOT;	//!<ピボット。
};

