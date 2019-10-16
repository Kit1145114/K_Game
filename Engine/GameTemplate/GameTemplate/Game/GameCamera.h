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
private:
	//CVector3�^�̃|�W�V�����̏������������o�ϐ�
	CVector3 m_position = CVector3::Zero();	//�����ʒu�i0�j�����B
	Player* m_player = nullptr;
};

