#pragma once
class Player;
class PhysicsGhostObject;

/// <summary>
/// プレイヤーのイベントリスナーのインターフェースクラスです。
/// </summary>
/// <remark>
/// このクラスはObserverパターンのObserver役のクラスです。
/// プレイヤーのイベントを監視したいクラスは、このクラスを継承して
/// インターフェースを実装してください。
/// </remark>
class IPlayerEventListener
{
public :
	virtual ~IPlayerEventListener() {}
	/// <summary>
	/// プレイヤークラスが死亡したときに呼ばれる処理。
	/// </summary>
	/// <param name="pl">プレイヤー</param>
	virtual void OnPlayerDead( Player* pl) {}
	/// <summary>
	/// プレイヤーがロックオンしている時常に呼ばれる処理。
	/// </summary>
	/// <param name="pl"></param>
	virtual void OnStartLockOn(Player* pl) {}
	/// <summary>
	/// プレイヤーがロックオンしていない時に常に呼ばれる処理。
	/// </summary>
	/// <param name="pl"></param>
	virtual void OnEndLockOn(Player* pl){}
	/// <summary>
	/// 攻撃コリジョンが発生したときのイベント
	/// </summary>
	/// <param name="colli">攻撃コリジョン</param>
	virtual void OnOccurredAttackCollision(PhysicsGhostObject& colli, const wchar_t* eventName,int m_playerAtkPoint){}
	/// <summary>
	/// ダメージを受けたときに呼ばれる処理。
	/// </summary>
	/// <param name="pl"></param>
	virtual void OnDamage(Player* pl) {};
};

