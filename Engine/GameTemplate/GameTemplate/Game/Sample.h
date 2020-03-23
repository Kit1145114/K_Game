#pragma once
#include"../Game/Chara/Enemys.h"
class Sample : public Enemys
{
public:

	enum SampleState
	{
		SSIdle,			//待機
		SSAttack		//攻撃
	};

	Sample();
	~Sample() {};
	void Update();
	void Draw();
	void Attack() {};					//攻撃
	void Damage(float Damage);			//DAMAGE
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	void State();
	void Track();
private:
	SkinModel SModel;						//model説明不要ッ！！！！！
	Animation s_anim;						//アニメ
	AnimationClip s_animClip[3];			//アニメクリップ
	SampleState s_state;
	EnemyInitParam prm;						//エネパラ。
	CVector3 e4_pos = { -200.0f,0.0f,0.0f };	//エネミーのポジしょん
	CVector3 m_scale = { 30.0f,30.0f,30.0f };	//エネミーの大きさ
};

