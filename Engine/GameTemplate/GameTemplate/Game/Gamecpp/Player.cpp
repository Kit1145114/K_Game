#include "stdafx.h"
#include "Player.h"
#include"GameConst.h"
#include"Game.h"

Player::Player()
{
	//cmoファイルの読み込み。
	Gmodel.Init(L"Assets/modelData/Player.cmo");		//プレイヤーの描画
	g_animClip[0].Load(L"Assets/animData/P_idle.tka");	//待機のロード
	g_animClip[0].SetLoopFlag(true);
	g_animClip[1].Load(L"Assets/animData/P_walk.tka");	//歩きのロード
	g_animClip[1].SetLoopFlag(true);
	g_animClip[2].Load(L"Assets/animData/P_ATK.tka");	//殴りのロード
	g_animClip[2].SetLoopFlag(false);
	//g_animClip[3].Load(L"Assets/animData/P_walk.tka");	//歩きのロード
	//g_animClip[3].SetLoopFlag(true);
	g_anim.Init(
		Gmodel,			//アニメーションを流すスキンモデル
						//これでアニメーションとスキンモデルを関連付けする。
		g_animClip,		//アニメーションクリップの配列。
		m_AnimClipNum		//アニメーションクリップの数
	);
	//よし、じゃあアニメーションイベントぶちこんでやるぜ。
	g_anim.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName)
	{
		OnAnimationEvent(clipName, eventName);
	});

	m_charaCon.Init(40.0f, 10.0f, m_position);			//キャラコンの設定（半径、高さ、初期位置。）
	HP = 100.0f;		//プレイヤーの初期体力。
	ATK = 80.0f;		//プレイヤーの攻撃力。
	DEF = 0.0f;		//プレイヤーの防御力。
	playerState = pl_idle;
}

Player::~Player()
{
}
//プレイヤーのアップデート。
void Player::Update()
{
	//プレイヤーの更新情報を下に記述。
	Draw();							//プレイヤーの描画を呼ぶ。
	if (playerState != pl_Death) {
		Move();							//プレイヤーの移動を呼ぶ。
		PlayerAttack();					//プレイヤーの攻撃類
		PlayerState();					//プレイヤーの状態を呼ぶ。
		Rotation();						//プレイヤーの回転を呼ぶ。
		//Track();						//プレイヤーが敵を探す。
		Forward();						//プレイヤーの前ベクトル取得。
	}
	g_anim.Update(0.025f * NSpeed);	//アニメーションをフレーム単位で描画。
			//ワールド行列の更新。
	Gmodel.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());
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
	//進む速さの値決め。
	float Speed = 500.0f;
	//左スティック受け取りマシーン
	float lStick_x = (g_pad[0].GetLStickXF());
	float lStick_y = (g_pad[0].GetLStickYF());
	//カメラの前方方向と右方向を取得。
	CVector3 cameraForward = g_camera3D.GetForward();
	CVector3 cameraRight = g_camera3D.GetRight();
	//XZ平面での前方方向、右方向に変換する。
	cameraForward.y = None;
	cameraForward.Normalize();
	cameraRight.y = None;
	cameraRight.Normalize();
	//XZ成分の移動速度をクリア。
	m_moveSpeed.x = None;
	m_moveSpeed.z = None;
	if (g_pad[0].IsPress(enButtonX) && playerState == pl_Walk) {
		//走る
		m_moveSpeed += cameraForward * lStick_y * Speed * SPeed2;	//奥方向への移動速度を加算。
		m_moveSpeed += cameraRight * lStick_x * Speed * SPeed2;		//右方向への移動速度を加算。
	}
	else
	{
		//歩き。
		m_moveSpeed += cameraForward * lStick_y * Speed * NSpeed;	//奥方向への移動速度を加算。
		m_moveSpeed += cameraRight * lStick_x * Speed * NSpeed;		//右方向への移動速度を加算。
	}
	//キャラクターコントローラーに１フレームの経過秒数、時間ベースの移動速度を渡している。
	m_charaCon.SetPosition(m_position);		//キャラコンに座標を渡す。
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
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
	switch (playerState)
	{
	case pl_idle:	//待機状態
		MoveOperation();
		if (m_moveSpeed.x != None || m_moveSpeed.z != None)
		{
			playerState = pl_Walk;	//歩きにチェンジ。
		}
		g_anim.Play(0);
		break;
	case pl_Walk:	//歩き状態。
		MoveOperation();
		if (m_moveSpeed.x == None && m_moveSpeed.z == None)
		{
			playerState = pl_idle;
		}
		g_anim.Play(1);
		break;
	case pl_Atk:	//攻撃状態。
		m_moveSpeed.z = None;
		m_moveSpeed.x = None;
		g_anim.Play(2);
		if (g_anim.IsPlaying() == false)
		{
			playerState = pl_idle;
		}
		break;
	case pl_Death:	//死亡状態
		g_anim.Play(0);
		break;
	}
}
//プレイヤーの攻撃類。
void Player::PlayerAttack()
{
	//もしBボタンが押されたら、パンチ。
	//攻撃モーションとゴーストの当たり判定を生成。
	if (g_pad[0].IsTrigger(enButtonB))
	{
		playerState = pl_Atk;
		m_PhyGhostObj.CreateBox(m_forward, m_rotation, m_scale);
	}
}
//プレイヤーの移動類。
void Player::MoveOperation()
{
		//パッドのABUTTON入力でジャンプする。
	if (g_pad[0].IsTrigger(enButtonA))
	{
		m_moveSpeed.y += JumpPower;
	}
	else
	{
		//Y方向への重力をつける。
		m_moveSpeed.y -= gravity; //* (1.0f / 60.0f);
	}
}
//アニメーションイベント
void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	float Kyori = 500.0f;
	for (auto enemy : m_goList) {
		if (enemy->GetIsDead() == false) {
			CVector3 diff = m_position - enemy->GetPosition();
			if (diff.Length() <= Kyori && eventName)
			{
				MessageBox(NULL, TEXT("Hit114514"), TEXT("めっせ"), MB_OK);
				enemy->Damage(ATK);
			}
		}
	}

}
//敵との距離計測とキル。
void Player::Track()
{
}
//前ベクトル
void Player::Forward()
{
	Rot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = Rot.m[2][0];
	m_forward.y = Rot.m[2][1];
	m_forward.z = Rot.m[2][2];
}
//DAMAGEの処理。
void Player::Damage(float Damage)
{
	HP -= (Damage - DEF);
	//もし、HPが0以下なら死亡処理。
	if (HP <= 0.0f)
	{
		playerState = pl_Death;
		MessageBox(NULL, TEXT("はい雑魚乙＾＾"), TEXT("めっせ"), MB_OK);
	}
}