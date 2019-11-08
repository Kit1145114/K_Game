#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	//cmoファイルの読み込み。
	Gmodel.Init(L"Assets/modelData/Gmodel.cmo");		//プレイヤーの描画
	g_animClip[0].Load(L"Assets/animData/G_idle.tka");	//待機のロード
	g_animClip[0].SetLoopFlag(true);
	g_animClip[1].Load(L"Assets/animData/G_walk.tka");	//歩きのロード
	g_animClip[1].SetLoopFlag(true);
	g_animClip[2].Load(L"Assets/animData/G_ATK.tka");	//殴りのロード
	g_animClip[2].SetLoopFlag(false);
	g_anim.Init(
		Gmodel,			//アニメーションを流すスキンモデル
						//これでアニメーションとスキンモデルを関連付けする。
		g_animClip,		//アニメーションクリップの配列。
		3				//アニメーションクリップの数
	);
	m_charaCon.Init(40.0f, 10.0f, m_position);			//キャラコンの設定（半径、高さ、初期位置。）
	HP = 1000.0f;		//プレイヤーの初期体力。
	ATK = 80.0f;		//プレイヤーの攻撃力。
	DEF = 50.0f;		//プレイヤーの防御力。
	plClip = plAnimClip_idle;
}

Player::~Player()
{
}
//プレイヤーのアップデート。
void Player::Update()
{
	//プレイヤーの更新情報を下に記述。
	Draw();							//プレイヤーの描画を呼ぶ。
	Move();							//プレイヤーの移動を呼ぶ。
	PlayerAttack();					//プレイヤーの攻撃類
	PlayerState();					//プレイヤーの状態を呼ぶ。
	Rotation();						//プレイヤーの回転を呼ぶ。
	g_anim.Update(1.0f / 30.0f);	//アニメーションをフレーム単位で描画。
}
//プレイヤーの描画処理。
void Player::Draw()
{
	Gmodel.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
}
//プレイヤーの移動処理
void Player::Move()
{
	//m_moveSpeedはプレイヤーのclassのメンバ変数。
	//キャラクターコントローラーに１フレームの経過秒数、時間ベースの移動速度を渡している。
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	m_charaCon.SetPosition(m_position);		//キャラコンに座標を渡す。
		//ワールド行列の更新。
	Gmodel.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
}
//プレイヤーの回転処理
void Player::Rotation()
{
	float None = 0.0f;		//マジックナンバーを防ぐ0を入れた数
	float Rot = atan2(m_moveSpeed.x, m_moveSpeed.z);
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisY(), Rot);
	Gmodel.SetRotation(qRot);
	//もし、動いていたら回転させる。
	if (m_moveSpeed.x != None || m_moveSpeed.z != None)
	{
		m_rotation = qRot;
		Gmodel.SetRotation(m_rotation);
	}
	if (m_moveSpeed.x == None && m_moveSpeed.z == None)
	{
		Gmodel.SetRotation(m_rotation);
	}
	Gmodel.SetRotation(m_rotation);
}
//プレイヤーの状態処理。
void Player::PlayerState()
{
	//もし、動いていないとき※後でわかりやすい書き方に変更予定。
	//プレイヤーの状態で処理の変更。
	switch (plClip)
	{
	case plAnimClip_idle:	//待機状態
		MoveOperation();
		if (m_moveSpeed.x != 0 || m_moveSpeed.z != 0)
		{
			plClip = plAnimClip_Walk;	//歩きにチェンジ。
		}
		g_anim.Play(0);
		break;
	case plAnimClip_Walk:	//歩き状態。
		MoveOperation();
		if (m_moveSpeed.x == 0.0f && m_moveSpeed.z == 0)
		{
			plClip = plAnimClip_idle;
		}
		g_anim.Play(1);
		break;
	case plAnimClip_Atk:	//攻撃状態。
		m_moveSpeed.z = 0.0f;
		m_moveSpeed.x = 0.0f;
		g_anim.Play(2);
		if (g_anim.IsPlaying() == false)
		{
			plClip = plAnimClip_idle;
		}
		break;
	}
}
//プレイヤーの攻撃類。
void Player::PlayerAttack()
{
	if (g_pad[0].IsTrigger(enButtonB))
	{
		plClip = plAnimClip_Atk;
	}
}
//プレイヤーの移動類。
void Player::MoveOperation()
{
	m_moveSpeed.x = g_pad[0].GetLStickXF() * 300.0f;	//X方向への移動処理。
	m_moveSpeed.z = g_pad[0].GetLStickYF() * 300.0f;	//Y方向への移動処理。
		//パッドのABUTTON入力でジャンプする。
	if (g_pad[0].IsTrigger(enButtonA))
	{
		m_moveSpeed.y += 400.0f;
	}
	else
	{
		//Y方向への重力をつける。
		m_moveSpeed.y -= 100.0f; //* (1.0f / 60.0f);
	}
}