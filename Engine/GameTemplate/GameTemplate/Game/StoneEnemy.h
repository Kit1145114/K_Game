#pragma once
#include"Enemys.h"
class StoneEnemy :public Enemys
{
public:
	StoneEnemy();
	~StoneEnemy(){};
	void Attack();							//攻撃(今は未実装)
	void Damage(float Damage) override;		//DAMAGE（今は未実装）
	void Search();							//プレイヤーを見つける関数
	void Update();							//敵の更新内容を記入
	void Draw();							//エネミーをドロー処理だよ。
	void Death();							//エネミーが死ぬ処理だよ。
	void EnemyState();						//エネミーをステイトで行動する処理だよ。
	void EMove();							//エネミーが動く処理だよ。
	void Rotation();						//エネミーが動きに合わせて回転する処理だよ。
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);		//アニメーションイベント。
private:
	Enemys* enemys;
	SkinModel seModel;						//model説明不要ッ！！！！！
	Animation s_anim;						//アニメ
	AnimationClip s_animClip[m_AnimClipNum];//アニメクリップ
};

