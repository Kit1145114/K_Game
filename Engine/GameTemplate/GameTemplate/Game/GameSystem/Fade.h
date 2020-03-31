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
		static Fade instance;		//�����v��

		return instance;
	}
private:
	Fade();
	~Fade();
private:
	enum EnState {
		enState_FadeIn,		//!<�t�F�[�h�C�����B
		enState_FadeOut,		//!<�t�F�[�h�A�E�g���B
		enState_Idle,		//!<�A�C�h�����B
	};
	Sprite m_sprite;
	//SpriteRender m_texture;
	EnState m_state = enState_Idle;	//!<��ԁB
	float m_currentAlpha = 1.0f;		//!<���݂̃��l�B
};

