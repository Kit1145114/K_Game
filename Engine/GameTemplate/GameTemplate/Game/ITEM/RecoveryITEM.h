#pragma once
#include"GameObjectManajer.h"
#include"../GameSystem/GameConst.h"
#include "character/CharacterController.h"
#include"Chara/Player.h"

class Player;
class RecoveryITEM : public GameObject
{
public:
	RecoveryITEM();
	~RecoveryITEM();
	bool Start();					//�n�܂�̏��B
	void Update();					//�X�V�����B
	void Death();					//���ʂƂ�...
	void Rotation();				//��]
	void Draw();					//�`���Ă܂��B
	void HealAcquisition();			//�v���C���[����ɓ���邽�߂̏����B
	void SetPositon(CVector3 pos)
	{
		m_position = pos;
	}
	//�v���C���[�̏���n���B
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
private:
	Player* m_player;
	CVector3 m_position = CVector3::Zero();
	CVector3 m_moveSpeed = CVector3::Zero();
	CVector3 Move = CVector3::Zero();
	CVector3 m_scale = { 3.0f,3.0f,3.0f };
	CQuaternion m_rotation = CQuaternion::Identity();
	SkinModel Item;
	Animation m_anim;
	AnimationClip item_animClip[1];	//�v���C���[�̃A�j���[�V�����N���b�v
	CharacterController m_charaCon;				//�L�����N�^�[�R���g���[���[
	float m_timer = 0.0f;
	float Limit = 1.0f;
	int m_healHp = 100;							//�񕜁B
};

