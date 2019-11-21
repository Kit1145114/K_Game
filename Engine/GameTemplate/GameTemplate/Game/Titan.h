#pragma once
#include"Enemys.h"

class Titan : public Enemys
{
public:
	Titan();
	~Titan() {};
	void Attack() override;					//攻撃(今は未実装)
	void Damage(float Damage) override;		//DAMAGE（今は未実装）
	void Search();							//プレイヤーを見つける関数
	void Update();
	void Draw();
private:
	SkinModel tModel;							//model説明不要ッ！！！！！
	Animation t_anim;							//アニメ
	AnimationClip t_animClip[3];				//アニメクリップ
	EnemyInitParam prm;							//エネパラ。
	CVector3 e3_pos = { -400.0f,0.0f,0.0f };	//エネミーの初期ポジ
};

