#pragma once
#include"ICollider.h"

class PhysicsGhostObject
{
public:
	~PhysicsGhostObject()
	{
		Release();
	}
	/*!
	* @brief	�S�[�X�g�I�u�W�F�N�g������B
	*@detail
	* �����I�ȃ^�C�~���O�ŃS�[�X�g�I�u�W�F�N�g���폜�������ꍇ�ɌĂяo���Ă��������B
	*/
	void Release();
	/*!
	* @brief	�����œn���ꂽ�S�[�X�g�I�u�W�F�N�g���������g���ǂ�������B
	*/
	bool IsSelf(const btCollisionObject& ghost) const
	{
		return &ghost == &m_ghostObject;
	}
	/*!
	* @brief	���W��ݒ�B
	*/
	void SetPosition(const CVector3& pos);
	/*!
	* @brief	��]��ݒ�B
	*/
	//void SetRotation(const CQuaternion& rot);
private:
	/*!
	* @brief	�S�[�X�g�쐬�����̋��ʏ����B
	*/
	void CreateCommon(CVector3 pos, CQuaternion rot); //override;
private:
	bool						m_isRegistPhysicsWorld = false;	//!<�������[���h�ɓo�^���Ă��邩�ǂ����̃t���O�B
	btGhostObject				m_ghostObject;	//!<�S�[�X�g
	std::unique_ptr<ICollider> m_collider;	//!<�R���C�_�[�B
};

