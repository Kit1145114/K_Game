#pragma once
#include"Chara/IPlayerEventListener.h"

class Player;
/// <summary>
/// �Q�[���J�����N���X
/// </summary>
/// <remark>
/// ���̃N���X��Observer�p�^�[����ConcreteObserver���̃N���X�ł��B
/// ���̃N���X�̓v���C���[�̏�Ԃ��Ď����Ă��܂��B
/// </remark>
class GameCamera : public GameObject ,IPlayerEventListener
{
public:
	/// <summary>
	/// �J�����̃X�e�[�g
	/// </summary>
	enum CameraState {
		toPlayer,
		toEnemys
	};

	/// <summary>
	/// �C���X�g���N�^
	/// </summar>
	GameCamera();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameCamera();
	bool Start()override;			//�v���C���[�̃C�x���g���X�i�[�o�^�����C���B
	void Update() override;			//�J�����̍X�V�֐��B
	void State();					//�J�����̏�ԁB
	/// <summary>
	/// �v���C���[�̍��W�������Ă���B
	/// </summary>
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	//�J�����C�F�A
	void PlayerDamageRot();
	/// <summary>
	/// �J�����̊p�x�ύX
	/// </summary>
	void CameraRotate();
	/// <summary>
	/// �J������G�Ƀ��b�N����B
	/// </summary>
	void CameraLookEnemys();
	/// <summary>
	/// �v���C���[�����b�N�I�����Ă��鎞��ɌĂ΂�鏈���B
	/// </summary>
	void OnStartLockOn(Player* pl)override;
	/// <summary>
	/// �v���C���[�����b�N�I�����Ă��Ȃ����ɏ�ɌĂ΂�鏈���B
	/// </summary>
	void OnEndLockOn(Player* pl)override;
	/// <summary>
	/// �v���C���[���_���[�W���󂯂��Ƃ��ɉ�ʂ��h��鏈���B
	/// </summary>
	void OnDamage(Player* pl)override;

	CVector3 GetPosition() const
	{
		return m_position;
	}
private:
	Player* m_player = nullptr;							//�v���C���[�N���X�̏������B
	CVector3 m_position = CVector3::Zero();				//�J�����̍��W
	CVector3 m_playerPosition = CVector3::Zero();		//�~�̒��S���W(�v���C���[�̍��W)
	CVector3 m_enemyPosition = CVector3::Zero();		//���b�N�I������G�l�~�[�̍��W�B
	CVector3 m_target = CVector3::Zero();				//�J�����̒����_
	CQuaternion m_rotate = CQuaternion::Identity();		//�J�����̉�]�B
	CameraState c_state;								//�J�����̃X�e�[�g�B
	CVector3 m_moveCamera = { 5.0f,0.0f,0.0f };
	//�J�����̍��W����ђ����_�̌v�Z�p�B
	void Hutu();
	CVector3 m_toPos = { 0.0f, 0.0f, 0.0f };
	//�p�x
	float m_degreexz = 0.0f;
	float m_degreey = 0.0f;
	//��]���x
	float m_sdegreexz = 0.0f;
	float m_sdegreey = 0.0f;
	//�ۑ��p�x
	float m_memoryDegreexz = 0.0f;
	float m_moveNum = 4.0f;
	bool damage_flag = false;
	float timer = 0.0f;
	//���a
	const float m_r = 250.0f;
	//�p�x�����W�A���P��(��)�ɒ��������
	double m_radianx;
	double m_radiany;
	//
	CVector3 m_right = CVector3::Zero();
};