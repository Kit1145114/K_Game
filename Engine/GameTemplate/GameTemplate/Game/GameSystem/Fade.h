#pragma once
#include"graphics/SpriteRender.h"

class Fade : public GameObject
{
public:
	bool Start() override;
	void Update() override;
	void Render() override;
	//�Â��Ȃ�n�߂�
	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}
	//���ɖ߂�
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
	Fade();
	~Fade();
private:
	enum EnState {
		enState_FadeIn,		//!<�t�F�[�h�C�����B
		enState_FadeOut,	//!<�t�F�[�h�A�E�g���B
		enState_Idle,		//!<�A�C�h�����B
	};
	SpriteRender* m_sprite;
	EnState m_state;	//!<��ԁB
	float m_currentAlpha = 0.0f;	//!<���݂̃��l�B
};

