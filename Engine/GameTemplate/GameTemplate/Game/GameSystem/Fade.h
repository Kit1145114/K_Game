#pragma once
#include"graphics/SpriteRender.h"

class Fade : public GameObject
{
public:
	bool Start() override;
	void Update() override;
	void Render() override;
	//暗くなり始める
	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}
	//元に戻る
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
	Fade();
	~Fade();
private:
	enum EnState {
		enState_FadeIn,		//!<フェードイン中。
		enState_FadeOut,	//!<フェードアウト中。
		enState_Idle,		//!<アイドル中。
	};
	SpriteRender* m_sprite;
	EnState m_state;	//!<状態。
	float m_currentAlpha = 0.0f;	//!<現在のα値。
};

