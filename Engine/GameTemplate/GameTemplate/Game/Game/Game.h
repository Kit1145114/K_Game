#pragma once
#include<unordered_map>
#include"graphics/c3dmodel/C3DModelDraw.h"
#include"graphics/RenderTarget.h"
#include"level/Level.h"
#include"GameSystem/GameConst.h"
#include"sound/SoundEngine.h"
#include"sound/SoundSource.h"
#include"Chara/IPlayerEventListener.h"

class Player;					//�v���C���[�p��class�̌^���쐬�B
class MAP;						//�}�b�v�p��class�̌^�𐶐��B
class GameCamera;				//�J�����p��class�̌^�𐶐��B
class Enemys;					//�G�l�~�[�p��class�̌^�𐶐��B
class HPText;					//HP�o�[�p��class
class EnergyText;				//�G�i�W�[�o�[�p��class
class ITEMBox;					//����class
class Door;						//�h�A��class�m�^
class Wall;						//�ǂ̌^
class Sky;						//���class�̌^�B

class Game : public GameObject, IPlayerEventListener
{

	enum EnRenderMode {
		enRenderMode_Normal,		//�ʏ탌���_�����O�B
		enRenderMode_Silhouette,	//�V���G�b�g�������_�����O�B
		enRenderMode_Num,			//�����_�����O���[�h�̐��B
	};

	enum EnemyType {
		RobbotoEnemy1,
		RobbotoEnemy2,
		treeGolem,
		BossEnemy
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
	bool FirstStage();				//�ŏ��̃X�e�[�W
	bool NewBoss();					//�{�X���ĂԊ֐�
	bool DebugStage();				//�f�o�b�N�p�̃X�e�[�W�B
	void Update();					//�Q�[���̃A�b�v�f�[�g�֐��B
	void FirstStageUpdate();		//�ŏ��̃X�e�[�W�ōs���A�b�v�f�[�g�B
	void BossStageUpdate();			//�{�X�̃X�e�[�W�ōs���A�b�v�Ł[��B
	void Walldelete();				//�ǂ̏���
	void GameScene();				//�Q�[���I�[�o�[���̏����B
	void PlayerBarUpdate();			//�v���C���[�̗̑́A�u�[�X�g�̃A�v�f�B
	//�G�ƂՂ�����[�̋����𑪂�
public:
	const void SetStage(int n) 
	{
		m_stage = n;
	}
	const void SetPlayerHp(float hp) 
	{
		m_playerHp = hp;
	}
	/// <summary>
	/// �v���C���[�����S�����Ƃ��ɌĂ΂�鏈���B
	/// </summary>
	/// <param name="pl"></param>
	void OnPlayerDead(Player* pl) override;
	/// <summary>
private:
	Player* m_player = nullptr;				//�v���C���[�̃C���X�^���X�B
	MAP* m_map = nullptr;					//�}�b�v�̃C���X�^���X�B
	GameCamera* m_camera = nullptr;			//�J�����̃C���X�^���X�B
	Enemys* m_enemys = nullptr;				//�G�l�~�[�̃C���X�^���X�B
	HPText* m_hpText = nullptr;				//HP�o�[�B
	ITEMBox* m_itemBox = nullptr;			//��
	EnergyText* m_energyText = nullptr;		//�G�i�W�[�o�[
	Door* m_door = nullptr;					//�h�A
	Wall* m_wall = nullptr;					//��
	Sky* m_sky = nullptr;					//��
	Level		mapLevel;					//�}�b�v�̃��x���B�B�B�B�B�B
	StageNum	stage;
	bool stageChange_flag = false;
	bool wallDeath_flag = false;			//�Ǎ폜�̃t���O�B
	bool deathEnemys1_Flag = false;			//�O���[�v�P�S�����񂾂�
	bool deathEnemys2_Flag = false;			//�O���[�v�Q�S�����񂾂�
	bool deathEnemys3_Flag = false;			//�O���[�v�R�S�����񂾂�
	bool deathEnemys4_Flag = false;			//�O���[�v�S�S�����񂾂�
	bool isWallDelete_Flag[4] = {false,false,false,false};
	int	m_stage = 0;						//�i�ރX�e�[�W
	int m_stageNum = 0;						//�Z�b�g����X�e�[�W�̔ԍ�
	int m_maxEnemys[4] = { 0 };				//�O���[�v�̍ő�l��
	float m_playerHp = 100.0f;				//�v���C���[�̏���HP
	static Game* m_instance;				//�Q�[���̃C���X�^���X�B
	const int ENEMY_NUM = 4;				//�o�ꂷ��G�l�~�[�̐��������Œ�`�B
		//�ϒ��z��
	std::vector<Enemys*>m_enemysToPlayerList;				//�Ǘp�̃G�l�~�[�I�u�W�F�N�g�̃��X�g
	std::unordered_map<int,Wall*>m_wallList;				//�ǂ̃��X�g
	CVector3 m_initPlayerPos = { 0.0f,500.0f,0.0f };
	CVector3 m_doorPos = { 0.0f,-350.0f,0.0f };
	//BGM
	CSoundSource m_bgm;
	//
	EnemyType e_type;
};