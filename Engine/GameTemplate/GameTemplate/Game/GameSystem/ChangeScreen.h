#pragma once
#include"graphics/SpriteRender.h"

class Game;
class ChangeScreen : public GameObject
{
public:
	ChangeScreen();
	~ChangeScreen();
	void Update() override;
	void Death();
private:
	//�X�e�[�W
	int m_stageNum;
	//�X�v���C�g
	SpriteRender* m_sprite;
	//�Q�[������I�I
	Game* game;
	//�Ђ낿
	CQuaternion Rot = { 0.0f,1.0f,0.0f,0.0f };
};

