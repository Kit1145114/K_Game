#pragma once
#include"GameObjectManajer.h"
#include"graphics/c3dmodel/C3DModelDraw.h"
#include"RenderTarget.h"
#include"level/Level.h"
#include"GameConst.h"
#include"sound/SoundEngine.h"
#include"sound/SoundSource.h"

class Player;					//�v���C���[�p��class�̌^���쐬�B
class MAP;						//�}�b�v�p��class�̌^�𐶐��B
class GameCamera;				//�J�����p��class�̌^�𐶐��B
class Enemys;					//�G�l�~�[�p��class�̌^�𐶐��B
class HPText;					//HP�o�[�p��class
class EnergyText;				//�G�i�W�[�o�[�p��class
class ITEMBox;					//����class
class Door;
class Game : public GameObject
{

	enum EnRenderMode {
		enRenderMode_Normal,		//�ʏ탌���_�����O�B
		enRenderMode_Silhouette,	//�V���G�b�g�������_�����O�B
		enRenderMode_Num,			//�����_�����O���[�h�̐��B
	};
	//�X�e�[�W�̐U�蕪���B
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
	bool NewBoss();					//�{�X���ĂԊ֐�
	void Update();					//�Q�[���̃A�b�v�f�[�g�֐��B
	//bool isLive = false;			//�����Ă邩�ǂ���
	//�G�ƂՂ�����[�̋����𑪂�
private:
	Player* player;					//�v���C���[�̃C���X�^���X�B
	MAP* map;						//�}�b�v�̃C���X�^���X�B
	GameCamera* g_Camera;			//�J�����̃C���X�^���X�B
	Enemys* enemys;					//�G�l�~�[�̃C���X�^���X�B
	HPText* hp_bar;					//HP�o�[�B
	ITEMBox* itemBox;				//��
	EnergyText* energy_bar;			//�G�i�W�[�o�[
	Door*	door;					//�h�A
	Level		mapLevel;			//�}�b�v�̃��x���B�B�B�B�B�B
	int			m_stage;			//�X�e�[�W
	bool		StageChange = false;
	//bool		isLive = false;
	static Game* m_instance;		//�Q�[���̃C���X�^���X�B
	const int ENEMY_NUM = 4;		//�o�ꂷ��G�l�~�[�̐��������Œ�`�B
		//�ϒ��z��
	std::vector<Enemys*>m_enemysList;	//�G�l�~�[�I�u�W�F�N�g�̃��X�g
	std::list<Enemys*>enemysList;		//�G�l�~�[�̃��X�g�B
	CVector3 m_initPlayerPos = { 0.0f,500.0f,0.0f };
	CSoundEngine m_soundEngine;
	CSoundSource m_bgm;
};

