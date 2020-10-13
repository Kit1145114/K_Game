#pragma once

class Player;
class Door : public GameObject
{
public:
	Door();
	~Door();
	void Update();					//�X�V�����B
	void Draw();					//�`�揈���B
	void PlayerDistance();			//�v���C���[���ڋ߂��Ĕ��ɓ��鏈���B

public:
	//�O������position��ݒ�B
	const void SetPosition(CVector3& pos)
	{
		m_position = pos;
	}
	void SetPlayer(Player* pl)
	{
		m_player = pl;
	};

	const CVector3& GetPosition()
	{
		return m_position;
	}
	const bool& GetChangeSta()
	{
		return isChangeStage_flag;
	}
private:
	Player* m_player;
	SkinModel Gate;							//�}�b�v�̃X�L�����f���B
	CVector3 m_position = CVector3::Zero();			//�|�W�V����
	CVector3 m_scale = CVector3::One();			//�X�P�[���B
	CQuaternion m_rotation = CQuaternion::Identity();	//���[�e�[�V�����B
	CVector3 m_distance = CVector3::Zero();
	float m_warpDistance = 125.0f;
	bool isChangeStage_flag = false;
};

