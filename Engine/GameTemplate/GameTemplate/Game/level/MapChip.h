#pragma once

#include "physics/PhysicsStaticObject.h"

class Level;
struct LevelObjectData;

/*!
* @brief	�}�b�v�`�b�v�B
*/
class MapChip {
public:
	/*!
	* @brief	�R���X�g���N�^�B
	*@param[in] objData			�I�u�W�F�N�g���B
	*/
	MapChip(const LevelObjectData& objData);
	/*!
	* @brief	�`��B
	*/
	void Draw();

	void Update()
	{
		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	}
private:
	SkinModel m_model;		//!<���f���B
	PhysicsStaticObject m_physicsStaticObject;	//!<�ÓI�����I�u�W�F�N�g
	CVector3 m_position;
	CQuaternion m_rotation;
	CVector3 m_scale;
	
};