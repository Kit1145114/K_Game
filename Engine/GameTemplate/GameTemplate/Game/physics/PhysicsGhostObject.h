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
	* @brief	ゴーストオブジェクトを解放。
	*@detail
	* 明示的なタイミングでゴーストオブジェクトを削除したい場合に呼び出してください。
	*/
	void Release();
	/*!
	* @brief	引数で渡されたゴーストオブジェクトが自分自身かどうか判定。
	*/
	bool IsSelf(const btCollisionObject& ghost) const
	{
		return &ghost == &m_ghostObject;
	}
	/*!
	* @brief	座標を設定。
	*/
	void SetPosition(const CVector3& pos);
	/*!
	* @brief	回転を設定。
	*/
	//void SetRotation(const CQuaternion& rot);
private:
	/*!
	* @brief	ゴースト作成処理の共通処理。
	*/
	void CreateCommon(CVector3 pos, CQuaternion rot); //override;
private:
	bool						m_isRegistPhysicsWorld = false;	//!<物理ワールドに登録しているかどうかのフラグ。
	btGhostObject				m_ghostObject;	//!<ゴースト
	std::unique_ptr<ICollider> m_collider;	//!<コライダー。
};

