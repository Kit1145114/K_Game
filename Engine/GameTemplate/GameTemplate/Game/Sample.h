#pragma once
#include"Enemys.h"
#include"Player.h"
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
	void Damage(float Damage);	
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);//DAMAGE
	void State();
	void Track();
	//void SetPlayer(Player* player)
	//{
	//	m_player = player;
	//}
private:
	SkinModel SModel;						//model説明不要ッ！！！！！
	Animation s_anim;						//アニメ
	AnimationClip s_animClip[3];			//アニメクリップ
	SampleState s_state;
	EnemyInitParam prm;						//エネパラ。
	CVector3 e4_pos = { -200.0f,0.0f,0.0f };
	CVector3 m_scale = { 30.0f,30.0f,30.0f };

	//Player* m_player = nullptr;
};

