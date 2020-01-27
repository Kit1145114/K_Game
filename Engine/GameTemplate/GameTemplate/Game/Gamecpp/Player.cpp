#include "stdafx.h"
#include "Player.h"
#include"GameConst.h"
#include"Game.h"
#define _USE_MATH_DEFINES //M_PI 円周率呼び出し
#include <math.h> 

Player::Player()
{
	//cmoファイルの読み込み。
	Gmodel.Init(L"Assets/modelData/Player.cmo");		//プレイヤーの描画
	g_animClip[0].Load(L"Assets/animData/P_idle.tka");	//待機のロード
	g_animClip[0].SetLoopFlag(true);
	g_animClip[1].Load(L"Assets/animData/P_walk.tka");	//歩きのロード
	g_animClip[1].SetLoopFlag(true);
	g_animClip[2].Load(L"Assets/animData/P_FlyMove.tka");	//ブーストのロード
	g_animClip[2].SetLoopFlag(true);
	g_animClip[3].Load(L"Assets/animData/P_ATK.tka");	//殴りのロード
	g_animClip[3].SetLoopFlag(false);
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

	m_charaCon.Init(100.0f, 250.0f, m_position);			//キャラコンの設定（半径、高さ、初期位置。）
	HP = 100.0f;		//プレイヤーの初期体力。
	ATK = 100.0f;		//プレイヤーの攻撃力。
	DEF = 500.0f;		//プレイヤーの防御力。
	ENERGY = 300.0f;	//プレイヤーのブースト量。
	playerState = pl_idle;
	playerENE = ene_Full;
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
		Energy();
		Move();							//プレイヤーの移動を呼ぶ。
		PlayerAttack();					//プレイヤーの攻撃類
		PlayerState();					//プレイヤーの状態を呼ぶ。
		Rotation();						//プレイヤーの回転を呼ぶ。
		//Track();						//プレイヤーが敵を探す。
		Forward();						//プレイヤーの前ベクトル取得。
		RookOnEnemys();					//エネミーをターゲティングする用。
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
	float Speed = 750.0f;
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
	if (g_pad[0].IsPress(enButtonX) && playerENE == ene_Full) {
		//走る
		m_moveSpeed += cameraForward * lStick_y * Speed * SPeed2;	//奥方向への移動速度を加算。
		m_moveSpeed += cameraRight * lStick_x * Speed * SPeed2;		//右方向への移動速度を加算。
	}
	else if(!g_pad[0].IsPress(enButtonX)|| playerENE == ene_Charge)
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
		g_anim.Play(0);
		break;
	case pl_Walk:	//歩き状態。
		MoveOperation();
		g_anim.Play(1);
		break;
	case pl_FlyMove:
		MoveOperation();
		g_anim.Play(2);
		break;
	case pl_Atk:	//攻撃状態。
		m_moveSpeed.z = ZERO;
		m_moveSpeed.x = ZERO;
		g_anim.Play(3);
		if (g_anim.IsPlaying() == false)
		{
			playerState = pl_idle;
		}
		break;
	case pl_Death:	//死亡状態
		g_anim.Play(0);
		MessageBox(NULL, TEXT("はい雑魚乙＾＾"), TEXT("めっせ"), MB_OK);
		break;
	}
}
//プレイヤーの攻撃類。
void Player::PlayerAttack()
{
	//もしBボタンが押されたら、パンチ。
	//攻撃モーションとゴーストの当たり判定を生成。
	if (g_pad[0].IsTrigger(enButtonY))
	{
		playerState = pl_Atk;
		CVector3 A = m_position + (m_forward * 150.0f);
		A.y += 150.0f;
		m_PhyGhostObj.CreateBox(A, m_rotation, box_scale);
	}
}
//プレイヤーの移動類。
void Player::MoveOperation()
{
		//パッドのABUTTON入力で上昇する。
	if (g_pad[0].IsPress(enButtonA) && playerENE == ene_Full)
	{
		m_moveSpeed.y += JumpPower;
	}
	else 
	{
		//まず上昇した分のスピードをゼロへ。
		if (!g_pad[0].IsPress(enButtonA) && m_moveSpeed.y >= ZERO) {
			//Y方向への重力をつける。
			m_moveSpeed.y = ZERO; //* (1.0f / 60.0f);
		}
		//ここで落下
		if(!g_pad[0].IsPress(enButtonA) || playerENE == ene_Charge)
		{
			m_moveSpeed.y -= gravity * 1.5;
		}
	}
	//動いてないときは待機に。
	if (m_moveSpeed.x == ZERO && m_moveSpeed.z == ZERO)
	{
		playerState = pl_idle;
	}
	//浮遊移動とき。
	else if (m_moveSpeed.x != ZERO && m_moveSpeed.z != ZERO 
		&& g_pad[0].IsPress(enButtonX)&& playerENE == ene_Full)
	{
		playerState = pl_FlyMove;
	}
	//歩きの時。
	else if (m_moveSpeed.x != ZERO && m_moveSpeed.z != ZERO || playerENE == ene_Charge)
	{
		playerState = pl_Walk;
	}
}
//アニメーションイベント
void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	float Kyori = 500.0f;
	for (auto enemy : m_enemysList) {
		if (enemy->GetIsDead() == false) {
			g_physics.ContactTest(enemy->GetCharaCon(), [&](const btCollisionObject& contactObject) {
				if (m_PhyGhostObj.IsSelf(contactObject) == true && eventName){
						MessageBox(NULL, TEXT("Hit114514"), TEXT("めっせ"), MB_OK);
						enemy->Damage(ATK);
				}
			});
		}
	}
}
//敵との距離計測とキル。(未使用)
void Player::Track()
{
}
//前ベクトル
void Player::Forward()
{
	auto mRot = CMatrix::Identity();
	mRot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = mRot.m[2][0];
	m_forward.y = mRot.m[2][1];
	m_forward.z = mRot.m[2][2];
}
//DAMAGEの処理。
void Player::Damage(int Damage)
{
	//ダメージが0s以下なら受けない
	if((Damage - DEF) >= ZERO)
	//ダメージを食らった...。
	HP -= (Damage - DEF);
	//もし、HPが0以下なら死亡処理。
	if (HP <= 0.0f)
	{
		m_isdeath = true;
	}
	//You are Dead。
	if (m_isdeath)
	{
		playerState = pl_Death;
	}
}
//エネルギーに関する処理。
void Player::Energy()
{
	//エナジーが0になったらモード変更。
	if (ENERGY <= MINENERGY)
	{
		playerENE = ene_Charge;
	}
	//chargeモード＆エナジーがマックスになるまで
	if (ENERGY < MAXENERGY && playerENE == ene_Charge)
	{
		ENERGY += ENERGYFLUCT / 3;
	}
	//エナジーがフルになったら通常モード
	if (ENERGY >= MAXENERGY)
	{
		playerENE = ene_Full;
	}
	if (ENERGY > MINENERGY && playerENE == ene_Full)
	{
		//MIN規定値以下＆浮遊移動中。
		if (playerState == pl_FlyMove)
		{
			ENERGY -= ENERGYFLUCT;
		}
		else if (g_pad[0].IsPress(enButtonA))
		{
			ENERGY -= ENERGYFLUCT/1.5;
		}
		//MAX規定値以下＆待機中。
		else if (playerState == pl_idle && ENERGY < MAXENERGY && !g_pad[0].IsPress(enButtonA))
		{
			ENERGY += ENERGYFLUCT;
		}
		//MAX規定値以下＆歩き中＆地上にいるとき。
		else if (playerState == pl_Walk && ENERGY < MAXENERGY && !g_pad[0].IsPress(enButtonA))
		{
			ENERGY += ENERGYFLUCT;
		}
	}

}
//敵をロックオンするための処理。
void Player::RookOnEnemys()
{
	//プレイヤーの向いている角度の計算。
	float degreep = atan2(m_forward.x, m_forward.z);
	//計算して出た暫定的に一番小さい角度を記憶する変数。
	float degreemum = M_PI * 2;
	//範囲外。
	float unRange = 1500.0;
	float distance = 750.0f;
	for (Enemys* enemys : m_enemysList)
	{
		CVector3 pos = m_position - enemys->GetPosition();
		diff = m_position - enemys->GetPosition();
		//プレイヤーとエネミーの距離が言って以外だったら下の処理をスキップ。
		if (pos.Length() >= unRange)
			continue;
		//プレイヤーとエネミーを結ぶベクトルを出す。
		CVector3 pos2 = enemys->GetPosition() - m_position;
		m_enemyPos = enemys->GetPosition();
		//y座標、すなわち高さを0に。
		pos2.y = 0.0f;
		//ベクトルを正規化する。
		pos2.Normalize();
		//プレイヤーとエネミーを結ぶベクトルの角度を計算します
		float degree = atan2f(pos2.x, pos2.z);
		//「プレイヤーの正面のベクトル角度、
		//「プレイヤーとエネミーを結ぶベクトルの角度の計算。
		if (M_PI <= (degreep - degree)) {
			degree = degreep - degree + M_PI * 2;
		}
		else if (-M_PI >= (degreep-degree)){
		degree = degreep - degree + M_PI * 2;
		}
		else
		{
			degree = degreep - degree;
		}
		//求めた角度にプレイヤーとエネミーの距離に応じて補正をかける。
		//距離が長いほど補正は大きい(値が大きくなります)
		degree = degree + degree * (pos.Length() / unRange)* 0.3f;
		//求めた値を比較していき、一番小さい値を決めていきます。
		if (fabs(degreemum) >= fabs(degree))
		{
			degreemum = degree;
		}
	}	
	//求めた一番小さい値が一定値より小さい場合、ターゲティングをオンにする。
	if (fabs(degreemum) <= M_PI/3&& g_pad[0].IsTrigger(enButtonB)
		&& diff.Length() < unRange) {
		m_isRookOn = true;
	}
	//一定値より大きい場合、ターゲティングをオフにする。
	else if(diff.Length() > unRange && m_isRookOn){
		m_isRookOn = false;
	}
	else if (m_isRookOn && g_pad[0].IsTrigger(enButtonB))
	{
		m_isRookOn = false;
	}
}