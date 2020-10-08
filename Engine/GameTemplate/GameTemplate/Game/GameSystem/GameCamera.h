#pragma once

class Player;
class GameCamera : public GameObject
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
	void Update() override;			//�J�����̍X�V�֐��B
	void State();			//�J�����̏�ԁB
	/// <summary>
	/// �v���C���[�̂��Ђ傤�����Ă����B
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

	void SetDamegeFlag(bool flag)
	{
		damage_flag = flag;
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