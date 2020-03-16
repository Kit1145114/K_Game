#pragma once
#include"GameObjectManajer.h"
#include"../Game/graphics/SpriteRender.h"
#include"GameConst.h"

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
	/// <summary>
	/// �X�V�֐��B
	/// </summary>
	void Update();
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
};

