#pragma once
#include"../Game/Chara/Enemys.h"
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
	void Damage(float Damage);			//DAMAGE
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	void State();
	void Track();
private:
	SkinModel SModel;						//model�����s�v�b�I�I�I�I�I
	Animation s_anim;						//�A�j��
	AnimationClip s_animClip[3];			//�A�j���N���b�v
	SampleState s_state;
	EnemyInitParam prm;						//�G�l�p���B
	CVector3 e4_pos = { -200.0f,0.0f,0.0f };	//�G�l�~�[�̃|�W�����
	CVector3 m_scale = { 30.0f,30.0f,30.0f };	//�G�l�~�[�̑傫��
};

