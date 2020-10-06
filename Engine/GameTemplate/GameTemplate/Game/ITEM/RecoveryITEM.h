#pragma once
#include"Game/GameObjectManager.h"
#include"../GameSystem/GameConst.h"
#include "character/CharacterController.h"
#include"Chara/Player.h"

class Player;
class RecoveryITEM : public GameObject
{
public:
	RecoveryITEM();
	~RecoveryITEM();
	bool Start()override;					//始まりの書。
	void Update()override;					//更新処理。
	void Death();					//死ぬとき...
	void Rotation();				//回転
	void HealAcquisition();			//プレイヤーが手に入れるための処理。
	void Draw() override;
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
	CVector3 m_diss = CVector3::Zero();
	CVector3 Move = CVector3::Zero();
	CVector3 m_scale = { 2.0f,2.0f,2.0f };
	CQuaternion m_rotation = CQuaternion::Identity();
	SkinModel Item;
	Animation m_anim;
	AnimationClip item_animClip[1];	//プレイヤーのアニメーションクリップ
	CharacterController m_charaCon;				//キャラクターコントローラー
	float m_timer = 0.0f;
	float Limit = 1.0f;
	float m_playerGetDiss = 125.0f;
	int m_healHp = 1;							//回復。
	int m_healCount = 0;
	int m_maxHeal = 50;							//回復上限でししししし。
};

