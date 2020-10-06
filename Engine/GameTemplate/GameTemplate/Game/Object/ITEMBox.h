#pragma once
#include"Game/GameObjectManager.h"
#include"physics/PhysicsStaticObject.h"
#include"../GameSystem/GameConst.h"
#include "character/CharacterController.h"
#include"ITEM/RecoveryITEM.h"
#include"Chara/Player.h"

class RecoveryITEM;
class Player;
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
	//開いてますか？
	bool GetIsOpen()
	{
		return isOpen;
	}
	//キャラコンの取得。
	CharacterController& GetCharaCon()
	{
		return m_charaCon;
	}
public:
	//ポジションを渡す用。
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	//大きさを渡す用
	void SetScale(CVector3 scale)
	{
		m_scale = scale;
	}
	//プレイヤーの情報を渡す。
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
private:
	RecoveryITEM* RItem;
	Player* m_player;
	SkinModel	Box;							//マップのスキンモデル。
	Animation box_anim;							//アニメーション。
	AnimationClip box_animClip[m_AnimClipNum];	//プレイヤーのアニメーションクリップ
	boxstate state;								//状態。
	//PhysicsStaticObject m_physicsStaticObj;		//静的オブジェクト
	CharacterController m_charaCon;				//キャラクターコントローラー

	CVector3 m_position		= CVector3::Zero();			//ポジション
	CVector3 m_scale =		CVector3::Zero();			//スケール。
	CQuaternion m_rotation = CQuaternion::Identity();	//ローテーション。
	bool isOpen = false;						//箱のフラグ
	bool itemDrop_flag = false;					//アイテムが出たか。
	float m_timer = 0.0f;						//時間
	float m_deathTime = 1.0f;					//箱が開封されてから消えるまでの時間。

		//エフェクト
	Effekseer::Effect* m_effect[3] = { nullptr };
	Effekseer::Handle m_playEffectHandle = 10;
};

