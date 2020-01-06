#pragma once
#include"GameObjectManajer.h"
class Player;
class GameCamera : public GameObject
{
public:
	/// <summary>
	/// �C���X�g���N�^
	/// </summar>
	GameCamera();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameCamera();
	void Update();		//�J�����̍X�V�֐��B
	/// <summary>
	/// �v���C���[�̂��Ђ傤�����Ă����B
	/// </summary>
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	/// <summary>
	/// �J�����̊p�x�ύX
	/// </summary>
	void CameraRotate();
private:
	//CVector3�^�̃|�W�V�����̏������������o�ϐ�
	CVector3 m_position = CVector3::Zero();	//�����ʒu�i0�j�����B
	CQuaternion m_rotate = CQuaternion::Identity();
	Player* m_player = nullptr;
	//�J�����̍��W����ђ����_�̌v�Z
	void Hutu();
	CVector3 m_toPos = { 0.0f, 0.0f, 0.0f };
	//�~�̒��S���W(�v���C���[�̍��W)
	CVector3 m_playerposition;
	//�p�x
	float m_degreexz = 0.0f;
	float m_degreey = 0.0f;
	//��]���x
	float m_sdegreexz = 0.0f;
	float m_sdegreey = 0.0f;
	//���a
	const float m_r = 170.0f;
	//�p�x�����W�A���P��(��)�ɒ��������
	double m_radianx;
	double m_radiany;
	//�����_
	CVector3 m_target;
	//�J�����̍��W
	//CVector3 m_position;
};