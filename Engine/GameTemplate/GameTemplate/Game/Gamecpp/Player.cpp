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
	g_anim.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName)
	{
		OnAnimationEvent(clipName, eventName);
	});

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
	//Track();						//プレイヤーが敵を探す。
	Forward();						//プレイヤーの前ベクトル取得。
	g_anim.Update(0.05f * Hasiru);	//アニメーションをフレーム単位で描画。
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
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//XZ成分の移動速度をクリア。
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	m_moveSpeed += cameraForward * lStick_y * Speed * Hasiru;	//奥方向への移動速度を加算。
	m_moveSpeed += cameraRight * lStick_x * Speed * Hasiru;		//右方向への移動速度を加算。
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
	//もしBボタンが押されたら、パンチ。
	//攻撃モーションとゴーストの当たり判定を生成。
	if (g_pad[0].IsTrigger(enButtonB))
	{
		plClip = plAnimClip_Atk;
		m_PhyGhostObj.CreateBox(m_forward, m_rotation, m_scale);
	}
}
//プレイヤーの移動類。
void Player::MoveOperation()
{
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
	if (g_pad[0].IsPress(enButtonX) && plClip == plAnimClip_Walk)
	{
		Hasiru = 2.0f;
	}
	else
	{
		Hasiru = 1.0f;
	}
}
//アニメーションイベント
void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//(void)clipName;
	//if (eventName)
	//{
	//	MessageBox(NULL, TEXT("..！！"), TEXT("めっせ"), MB_OK);
	//	//m_PhyGhostObj.CreateBox(m_position, m_rotation, m_scale);
	//}

}
//敵との距離計測。
void Player::Track()
{
	CVector3 diff = m_position - enemys->GetPosition();
	if (diff.Length() <= 500.0f && plClip == plAnimClip_Atk)
	{
		g_goMgr.QutavaleyaAGO(enemys);
	}
}
//前ベクトル
void Player::Forward()
{
	Rot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = Rot.m[2][0];
	m_forward.y = Rot.m[2][1];
	m_forward.z = Rot.m[2][2];
}