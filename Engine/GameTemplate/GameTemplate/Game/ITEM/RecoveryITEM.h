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
	bool Start();					//始まりの書。
	void Update();					//更新処理。
	void Death();					//死ぬとき...
	void Rotation();				//回転
	void Draw();					//描いてます。
	void HealAcquisition();			//プレイヤーが手に入れるための処理。
	void SetPositon(CVector3 pos)
	{
		m_position = pos;
	}
	//プレイヤーの情報を渡す。
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
	AnimationClip item_animClip[1];	//プレイヤーのアニメーションクリップ
	CharacterController m_charaCon;				//キャラクターコントローラー
	float m_timer = 0.0f;
	float Limit = 1.0f;
	int m_healHp = 100;							//回復。
};

