#pragma once
#include"GameObjectManajer.h"
#include"Enemys.h"

class Player;					//�v���C���[�p��class�̌^���쐬�B
class MAP;						//�}�b�v�p��class�̌^�𐶐��B
class GameCamera;				//�J�����p��class�̌^�𐶐��B
class Enemys;					//�G�l�~�[�p��class�̌^�𐶐��B
class StoneEnemy;				//���G�l�~�[�B

class Game : public GameObject
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Game();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Game();
	static Game* GetInstance();	//�V���O���g���B
	bool Start();				//�Q�[���̃X�^�[�g�֐��B
	void Update();				//�Q�[���̃A�b�v�f�[�g�֐��B
private:
	Player* player;				//�v���C���[�̃C���X�^���X�B
	MAP* map;					//�}�b�v�̃C���X�^���X�B
	GameCamera* g_Camera;		//�J�����̃C���X�^���X�B
	Enemys* enemys[4];			//�G�l�~�[�̃C���X�^���X�B
	static Game* m_instance;	//�Q�[���̃C���X�^���X�B
	int enemyNum = 3;			//�o�ꂷ��G�l�~�[�̐��������Œ�`�B
};

