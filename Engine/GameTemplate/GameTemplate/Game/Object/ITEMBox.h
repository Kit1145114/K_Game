#pragma once
#include"Game/GameObjectManager.h"
#include"physics/PhysicsStaticObject.h"
#include"../GameSystem/GameConst.h"
#include "character/CharacterController.h"
#include"ITEM/RecoveryITEM.h"
#include"Chara/Player.h"
#include"Chara/IPlayerEventListener.h"

class RecoveryITEM;
class Player;
/// <summary>
/// アイテムボックスクラス
/// </summary>
/// <remark>
/// このクラスはObserverパターンのConcreteObserver役のクラスです。
/// このクラスはプレイヤーの状態を監視しています。
/// </remark>
class ITEMBox : public GameObject,IPlayerEventListener
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
		isOpen_flag = is;
	}
	//開いてますか？
	bool GetIsOpen()
	{
		return isOpen_flag;
	}
	//キャラコンの取得。
	CharacterController& GetCharaCon()
	{
		return m_charaCon;
	}
public:
	//ポジションを渡す用。
	const void SetPosition(CVector3& pos)
	{
		m_position = pos;
	}
	//大きさを渡す用
	const void SetScale(CVector3& scale)
	{
		m_scale = scale;
	}
	//プレイヤーの情報を渡す。
	void BindPlayer(Player* player);
	void OnOccurredAttackCollision(PhysicsGhostObject& colli, const wchar_t* eventName,
		int m_playerAtkPoint)override;

private:
	RecoveryITEM* RItem = nullptr;
	Player* m_player = nullptr;
	SkinModel	Box;							//マップのスキンモデル。
	Animation box_anim;							//アニメーション。
	AnimationClip box_animClip[m_AnimClipNum];	//プレイヤーのアニメーションクリップ
	boxstate state;								//状態。
	CharacterController m_charaCon;				//キャラクターコントローラー

	CVector3 m_position		= CVector3::Zero();			//ポジション
	CVector3 m_scale =		CVector3::Zero();			//スケール。
	CQuaternion m_rotation = CQuaternion::Identity();	//ローテーション。
	bool isOpen_flag = false;						//箱のフラグ
	bool isItemDrop_flag = false;					//アイテムが出たか。
	float m_timer = 0.0f;						//時間
	float m_deathTime = 1.0f;					//箱が開封されてから消えるまでの時間。
	CSoundSource m_openSound;						//音
		//エフェクト
	Effekseer::Effect* m_effect[3] = { nullptr };
	Effekseer::Handle m_playEffectHandle = 10;
	//リスナー
	std::list<IPlayerEventListener*>m_playerEventListenerList;
};

