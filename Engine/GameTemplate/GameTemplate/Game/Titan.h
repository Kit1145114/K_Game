#pragma once
#include"Enemys.h"

class Titan : public Enemys
{
public:
	Titan();
	~Titan() {};
	void Attack();					//攻撃(今は未実装)
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
	SkinModel tModel;							//model説明不要ッ！！！！！
	Animation t_anim;							//アニメ
	AnimationClip t_animClip[m_AnimClipNum];	//アニメクリップ
	EnemyInitParam prm;							//エネパラ。
	CVector3 e3_pos = { -400.0f,0.0f,0.0f };	//エネミーの初期ポジ
	CVector3 m_scale = { 30.0f,30.0f,30.0f };	//エネミーの大きさ用のメンバ変数。
};

