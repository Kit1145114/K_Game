#pragma once
#include"GameObjectManajer.h"
#include"graphics/Sprite.h"
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
	///�`��֐�
	/// </summary>
	void Draw(){};
	/// <summary>
	/// �I��
	/// </summary>
	 void Death();
	 const void SetStageNum(StageNum& num)
	 {
		 stagenum = num;
	 }
private:
	//�X�e�[�W
	int m_stageNum;
	StageNum stagenum;
	//�X�v���C�g
	SpriteRender* m_sprite;
	//�Q�[������I�I
	Game* game;
	//�Ђ낿
	CQuaternion Rot = {0.0f,1.0f,0.0f,0.0f};
};

