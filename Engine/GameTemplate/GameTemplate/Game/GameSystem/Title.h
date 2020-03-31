#pragma once
#include"graphics/SpriteRender.h"
#include"GameConst.h"
#include"sound/SoundEngine.h"
#include"sound/SoundSource.h"

class Game;
class Title : public GameObject
{
public:
	//�X�e�[�W�B
	/// <summary>
	/// �C���X�g���N�^�B
	/// </summary>
	Title();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~Title();
	//�X�^�[�g�֐��B
	bool Start();
	/// <summary>
	/// �X�V�֐��B
	/// </summary>
	void Update() override;
	/// <summary>
	/// �I��
	/// </summary>
	 void Death();

private:
	//�X�e�[�W
	StageNum Stage;
	//�X�v���C�g
	SpriteRender* m_sprite;
	//�Ђ낿
	CQuaternion Rot = { 0.0f,1.0f,0.0f,0.0f };
	//���y
	CSoundEngine m_soundEngine;
	CSoundSource m_bgm[2];
};

