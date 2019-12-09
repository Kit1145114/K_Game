#pragma once
#include"GameObjectManajer.h"
#include"graphics/c3dmodel/C3DModelDraw.h"
#include"RenderTarget.h"

class Player;					//�v���C���[�p��class�̌^���쐬�B
class MAP;						//�}�b�v�p��class�̌^�𐶐��B
class GameCamera;				//�J�����p��class�̌^�𐶐��B
class Enemys;					//�G�l�~�[�p��class�̌^�𐶐��B
class HPText;
class AgoSample;

class Game : public GameObject
{
	enum EnRenderMode {
		enRenderMode_Normal,		//�ʏ탌���_�����O�B
		enRenderMode_Silhouette,	//�V���G�b�g�������_�����O�B
		enRenderMode_Num,			//�����_�����O���[�h�̐��B
	};
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Game();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Game();
	
	static Game* GetInstance();		//�V���O���g���B
	bool Start();					//�Q�[���̃X�^�[�g�֐��B
	void Update();					//�Q�[���̃A�b�v�f�[�g�֐��B
private:
	Player* player;					//�v���C���[�̃C���X�^���X�B
	MAP* map;						//�}�b�v�̃C���X�^���X�B
	GameCamera* g_Camera;			//�J�����̃C���X�^���X�B
	Enemys* enemys[4];				//�G�l�~�[�̃C���X�^���X�B
	HPText* hp_bar;					//HP�o�[�B
	AgoSample* ago;					//�{�B
	static Game* m_instance;		//�Q�[���̃C���X�^���X�B
	const int ENEMY_NUM = 4;		//�o�ꂷ��G�l�~�[�̐��������Œ�`�B

	//�������烌���_�����O�^�[�Q�b�g�֌W�̃����o�ϐ��ł��B

	C3DModelDraw m_playerDraw;
	C3DModelDraw m_mapDraw;
	C3DModelDraw m_enemysDraw[4];
	C3DModelDraw m_AgoDraw;
};

