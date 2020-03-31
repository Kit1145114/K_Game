#pragma once
#include"GameObjectManajer.h"
#include"physics/PhysicsStaticObject.h"
#include"../GameSystem/GameConst.h"
#include "character/CharacterController.h"

class ITEMBox : public GameObject
{
public:
	ITEMBox();
	~ITEMBox();
	enum boxstate
	{
		open,
		close,
		death
	};
	bool Start() override;					//
	void Update() override;					//アップデート処理。
	void Draw();							//ドロー関数。
	void State();							//ステート
	void Delete();							//消す。
	void Open();							//開いてる時の処理
	void Close();							//閉める。
public:
	//開けるかどうか
	void SetIsOpen(bool is)
	{
		isOpen = is;
	}
	bool GetIsOpen()
	{
		return isOpen;
	}
	CharacterController& GetCharaCon()
	{
		return m_charaCon;
	}
private:
	SkinModel	Box;							//マップのスキンモデル。
	Animation box_anim;							//アニメーション。
	AnimationClip box_animClip[m_AnimClipNum];	//プレイヤーのアニメーションクリップ
	boxstate state;								//状態。
	PhysicsStaticObject m_physicsStaticObj;		//静的オブジェクト
	CharacterController m_charaCon;				//キャラクターコントローラー

	CVector3 m_position		= CVector3::Zero();			//ポジション
	CVector3 m_scale =		CVector3::Zero();			//スケール。
	CQuaternion m_rotation = CQuaternion::Identity();	//ローテーション。
	bool isOpen = false;						//箱のフラグ
};

