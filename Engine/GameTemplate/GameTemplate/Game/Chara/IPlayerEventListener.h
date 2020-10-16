#pragma once
class Player;
class PhysicsGhostObject;

/// <summary>
/// �v���C���[�̃C�x���g���X�i�[�̃C���^�[�t�F�[�X�N���X�ł��B
/// </summary>
/// <remark>
/// ���̃N���X��Observer�p�^�[����Observer���̃N���X�ł��B
/// �v���C���[�̃C�x���g���Ď��������N���X�́A���̃N���X���p������
/// �C���^�[�t�F�[�X���������Ă��������B
/// </remark>
class IPlayerEventListener
{
public :
	virtual ~IPlayerEventListener() {}
	/// <summary>
	/// �v���C���[�N���X�����S�����Ƃ��ɌĂ΂�鏈���B
	/// </summary>
	/// <param name="pl">�v���C���[</param>
	virtual void OnPlayerDead( Player* pl) {}
	/// <summary>
	/// �v���C���[�����b�N�I�����Ă��鎞��ɌĂ΂�鏈���B
	/// </summary>
	/// <param name="pl"></param>
	virtual void OnStartLockOn(Player* pl) {}
	/// <summary>
	/// �v���C���[�����b�N�I�����Ă��Ȃ����ɏ�ɌĂ΂�鏈���B
	/// </summary>
	/// <param name="pl"></param>
	virtual void OnEndLockOn(Player* pl){}
	/// <summary>
	/// �U���R���W���������������Ƃ��̃C�x���g
	/// </summary>
	/// <param name="colli">�U���R���W����</param>
	virtual void OnOccurredAttackCollision(PhysicsGhostObject& colli, const wchar_t* eventName,int m_playerAtkPoint){}
	/// <summary>
	/// �_���[�W���󂯂��Ƃ��ɌĂ΂�鏈���B
	/// </summary>
	/// <param name="pl"></param>
	virtual void OnDamage(Player* pl) {};
};

