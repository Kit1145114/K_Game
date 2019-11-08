#pragma once
#include"Enemys.h"
#include"Player.h"
class Sample : public Enemys
{
public:

	enum SampleState
	{
		SSIdle,			//�ҋ@
		SSAttack		//�U��
	};

	Sample();
	~Sample() {};
	void Update();
	void Draw();
	void Attack() {};					//�U��
	void Damage(float Damage);	
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);//DAMAGE
	void State();
	void Track();
	//void SetPlayer(Player* player)
	//{
	//	m_player = player;
	//}
private:
	SkinModel SModel;						//model�����s�v�b�I�I�I�I�I
	Animation s_anim;						//�A�j��
	AnimationClip s_animClip[3];			//�A�j���N���b�v
	SampleState s_state;
	EnemyInitParam prm;						//�G�l�p���B
	CVector3 e4_pos = { -200.0f,0.0f,0.0f };
	CVector3 m_scale = { 30.0f,30.0f,30.0f };

	//Player* m_player = nullptr;
};

