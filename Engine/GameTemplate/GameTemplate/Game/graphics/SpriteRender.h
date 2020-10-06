#pragma once
#include"graphics/Sprite.h"
#include"Game/GameObjectManager.h"

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
	void SetScale(const CVector3& scale)
	{
		m_scale = scale;
	}
	void AddScaleX(float x)
	{
		m_scale.x += x;
	}
	void SetActiveFlag(bool flag)
	{
		m_sprite.SetActiveFlag(flag);
	}
	void Update();

	void Render() override;

	void mulColorAlpha(const CVector4& mulColor)
	{
		m_mulColor = mulColor;
	}
	void SetAlpha(float alpha)
	{
		m_AlphaColor = alpha;
	}
private:
	bool m_isDraw3D = false;
	bool Active = true;
	Sprite	m_sprite;								//!<スプライト。
	CVector3 m_position = CVector3::Zero();
	CQuaternion m_rotation = {0.0f,1.0f,0.0f,0.0f};	//!<回転。
	CVector3 m_scale = CVector3::One();				//!<拡大率。
	CVector2 m_pivot = { 0.5f,0.5f };				//!<ピボット。
	CVector4 m_mulColor = CVector4::White();		//!<乗算カラー。
	float m_AlphaColor = 0.0f;
};

