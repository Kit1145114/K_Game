#include "stdafx.h"
#include "Wall.h"

Wall::Wall()
{

}

Wall::~Wall()
{
	m_charaCon.RemoveRigidBoby();
}

bool Wall::Start()
{
	m_wall.Init(L"Assets/modelData/Wall.cmo");							//モデルの呼び出し。
	m_se.Init(L"Assets/sound/gogogogo1.wav");							//効果音の呼び出し。
	m_physicsStaticObj.CreateMeshObject(m_wall, m_position, m_rotation);
	wallState = w_idle;													//
	w_animClip[w_idle].Load(L"Assets/animData/Wall_idle.tka");
	w_animClip[w_idle].SetLoopFlag(true);
	w_animClip[w_move].Load(L"Assets/animData/Wall_Move.tka");
	w_animClip[w_move].SetLoopFlag(true);
	w_anim.Init(
		m_wall,				//アニメーションを流すスキンモデル。
							//これでアニメーションとスキンモデルを関連付けする。
		w_animClip,			//アニメーションクリップの配列。
		m_AnimClipNum		//アニメーションクリップの数
	);
	return true;
}
//更新
void Wall::Update()
{
	Draw();
	State();
	//アニメーションの更新
	w_anim.Update(GameTime().GetFrameDeltaTime());
	//モデルの更新
	m_wall.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}
//ドロー
void Wall::Draw()
{
	m_wall.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		1
	);
}
//状態
void Wall::State()
{
	switch (wallState)
	{
	case Wall::w_idle:
		Idle();				//動いてない処理
		break;
	case Wall::w_move:
		Move();				//動くときの処理
		break;
	case Wall::w_death:
		Death();			//死ぬとき。
		break;
	}
}
//死
void Wall::Death()
{
	m_physicsStaticObj.RemoveRigidBoby();
	g_goMgr.DeleteGO(this);
}
//動く処理
void Wall::Move()
{
	//アニメーション再生。
	//w_anim.Play(w_move,0.1f);
	m_se.Play(false);
	m_position.y -= 20.0f;
	//ここでタイマーを回します。
	m_timer += GameTime().GetFrameDeltaTime();
	if (m_timer > 1.5f) {
		wallState = w_death;
	}
}
//動かないとき
void Wall::Idle()
{
	w_anim.Play(w_idle, 0.1f);
	if (m_move_flag)
	{
		wallState = w_move;
	}
}