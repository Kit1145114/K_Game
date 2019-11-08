#pragma once
#include"GameObjectManajer.h"
#include"graphics/Sprite.h"
#include"SpriteRender.h"

class Game;
class Title : public GameObject
{
public:
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
	///�`��֐�
	/// </summary>
	void Draw();
	/// <summary>
	/// �I��
	/// </summary>
	 void Death();
private:
	/// <summary>
	/// �X�v���C�g
	/// </summary>
	//Sprite m_title;
	/// <summary>
	/// �Q�[���̃C���X�^���X
	/// </summary>
	SpriteRender* m_sprite;
	Game* game;
	//�Ђ낿
	CQuaternion Rot = {0.0f,1.0f,0.0f,0.0f};
};

