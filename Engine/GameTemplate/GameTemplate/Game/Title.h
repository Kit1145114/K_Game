#pragma once
#include"GameObjectManajer.h"
#include"graphics/Sprite.h"

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
	Sprite m_title;
	/// <summary>
	/// �Q�[���̃C���X�^���X
	/// </summary>
	Game* game;
};

