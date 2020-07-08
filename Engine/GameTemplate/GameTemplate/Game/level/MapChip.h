#pragma once

#include "physics/PhysicsStaticObject.h"

class Level;
struct LevelObjectData;

/*!
* @brief	マップチップ。
*/
class MapChip {
public:
	/*!
	* @brief	コンストラクタ。
	*@param[in] objData			オブジェクト情報。
	*/
	MapChip(const LevelObjectData& objData);
	/*!
	* @brief	描画。
	*/
	void Draw();

	void Update()
	{
		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	}
private:
	SkinModel m_model;		//!<モデル。
	PhysicsStaticObject m_physicsStaticObject;	//!<静的物理オブジェクト
	CVector3 m_position;
	CQuaternion m_rotation;
	CVector3 m_scale;
	
};