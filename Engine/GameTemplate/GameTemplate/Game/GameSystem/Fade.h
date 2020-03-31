#pragma once
#include"graphics/SpriteRender.h"

class Fade : public GameObject
{
public:
	bool Start() override;
	void Update() override;
	void Render() override;
	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}
	void StartFadeOut()
	{
		m_state = enState_FadeOut;
	}
	bool IsFade() const
	{
		return m_state != enState_Idle;
	}
	float GetCurrentAlpha() const
	{
		return m_currentAlpha;
	}
	static Fade& GetInstance()
	{
		static Fade instance;		//ここプロ

		return instance;
	}
private:
	Fade();
	~Fade();
private:
	enum EnState {
		enState_FadeIn,		//!<フェードイン中。
		enState_FadeOut,		//!<フェードアウト中。
		enState_Idle,		//!<アイドル中。
	};
	Sprite m_sprite;
	//SpriteRender m_texture;
	EnState m_state = enState_Idle;	//!<状態。
	float m_currentAlpha = 1.0f;		//!<現在のα値。
};

