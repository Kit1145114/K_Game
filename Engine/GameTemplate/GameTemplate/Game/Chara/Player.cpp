#include "stdafx.h"
#include "Player.h"
#include"../GameSystem/GameConst.h"
#include"Game/Game.h"
#define _USE_MATH_DEFINES //M_PI 円周率呼び出し
#include <math.h> 
#include "graphics/shadow/ShadowMap.h"
#include"../GameSystem/GameCamera.h"

Player::Player()
{
	//効果音
	m_se[0].Init(L"Assets/sound/enemy_attack_00.wav");
	m_se[1].Init(L"Assets/sound/Boost.wav");
	m_se[2].Init(L"Assets/sound/swing.wav");
	m_se[3].Init(L"Assets/sound/PlayerWalk.wav");
	//Assets/sound/PlayerWalk.wav
	//cmoファイルの読み込み。
	Gmodel.Init(L"Assets/modelData/Player.cmo");		//プレイヤーの描画
	g_animClip[pl_idle].Load(L"Assets/animData/P_idle.tka");	//待機のロード
	g_animClip[pl_idle].SetLoopFlag(true);
	g_animClip[pl_Walk].Load(L"Assets/animData/P_walk.tka");	//歩きのロード
	g_animClip[pl_Walk].SetLoopFlag(true);
	g_animClip[pl_FlyMove].Load(L"Assets/animData/P_FlyMove.tka");	//ブーストのロード
	g_animClip[pl_FlyMove].SetLoopFlag(true);
	g_animClip[pl_Atk].Load(L"Assets/animData/P_ATK.tka");	//殴りのロード
	g_animClip[pl_Atk].SetLoopFlag(false);
	g_animClip[pl_Combo].Load(L"Assets/animData/PlayerCombo.tka");	//殴りのロード
	g_animClip[pl_Combo].SetLoopFlag(false);
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

	m_charaCon.Init(60.0f, 120.0f, m_position, enCollisionAttr_Character);			//キャラコンの設定（半径、高さ、初期位置。）
	HP = 100.0f;		//プレイヤーの初期体力。
	MaxHp = HP;			//最大値の更新。
	ATK = 100;		//プレイヤーの攻撃力。
	DEF = 50;		//プレイヤーの防御力。
	ENERGY = 300.0f;	//プレイヤーのブースト量。
	playerState = pl_idle;
	playerENE = ene_Full;
	m_attackEffect[0] = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/test.efk");
	m_attackEffect[1] = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/hit.efk");
	m_attackEffect[2] = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/Boost.efk");

	TextureData textureData;
	textureData.emissionFilePath = L"Resource/sprite/emission.dds";
	Gmodel.InitTexture(&textureData);
}

Player::~Player()
{
}
//プレイヤーのアップデート。
void Player::Update()
{
	//プレイヤーの更新情報を下に記述。
	if (playerState != pl_Death) {
		Energy();
		Move();							//プレイヤーの移動を呼ぶ。
		PlayerAttack();					//プレイヤーの攻撃類
		PlayerState();					//プレイヤーの状態を呼ぶ。
		Rotation();						//プレイヤーの回転を呼ぶ。
		Forward();						//プレイヤーの前ベクトル取得。
		RookOnEnemys();					//エネミーをターゲティングする用。
	}
	g_anim.Update(GameTime().GetFrameDeltaTime() * NSpeed);	//アニメーションをフレーム単位で描画。
			//ワールド行列の更新。
	Gmodel.UpdateWorldMatrix(m_position, m_rotation, {0.5f,0.5f,0.5f});

	g_graphicsEngine->GetShadowMap()->UpdateFromLightTarget(CVector3(m_position.x + 1000.0f, m_position.y + 1000.0f, m_position.z + 1000.0f),m_position);
}
//プレイヤーの描画処理。
void Player::Draw()
{
	Gmodel.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix(),
		1
	);
}
//プレイヤーのHP表示
void Player::FontRender()
{
	//場所と色の指定。
	m_font.DrawScreenPos(L"Player", CVector2(190, 40),
		CVector4(0.0f,1.0f,0.0f,1.0f));
	wchar_t text[10];
	swprintf_s(text, L"%d",HP);
	m_font.DrawScreenPos(text, CVector2(400, 40),
		CVector4(0.0f, 1.0f, 0.0f, 1.0f));
}
//プレイヤーの移動処理
void Player::Move()
{
	//進む速さの値決め。
	float Speed = 450.0f;
	//左スティック受け取りマシーン
	float lStick_x = (g_pad[0].GetLStickXF());
	float lStick_y = (g_pad[0].GetLStickYF());
	//カメラの前方方向と右方向を取得。
	cameraForward = g_camera3D.GetForward();
	cameraRight = g_camera3D.GetRight();
	//XZ平面での前方方向、右方向に変換する。
	cameraForward.y = None;
	cameraForward.Normalize();
	cameraRight.y = None;
	cameraRight.Normalize();
	//XZ成分の移動速度をクリア。
	m_moveSpeed.x = None;
	m_moveSpeed.z = None;
	if (playerState != pl_Atk ) {
		if (g_pad[0].IsPress(enButtonX) && playerENE == ene_Full) {
			//走る
			m_moveSpeed += cameraForward * lStick_y * Speed * RunSPeed;		//奥方向への移動速度を加算。
			m_moveSpeed += cameraRight * lStick_x * Speed * RunSPeed;		//右方向への移動速度を加算。
			SoundTrue(1);
		}
		else if (!g_pad[0].IsPress(enButtonX) || playerENE == ene_Charge)
		{
			//歩き。
			m_moveSpeed += cameraForward * lStick_y * Speed * NSpeed;	//奥方向への移動速度を加算。
			m_moveSpeed += cameraRight * lStick_x * Speed * NSpeed;		//右方向への移動速度を加算。
			g_effektEngine->Stop(m_playEffectHandle);
		}
	}
	else if (playerState == pl_Atk)
	{
		m_moveSpeed.z += m_forward.z * AttackMoveSpeed;		//前後方向への移動速度を加算。
		m_moveSpeed.x += m_forward.x * AttackMoveSpeed;		//左右方向への移動速度を加算。
		
	}
	//キャラクターコントローラーに１フレームの経過秒数、時間ベースの移動速度を渡している。
	//キャラコンに座標を渡す。
	m_charaCon.SetPosition(m_position);	
	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
}
//プレイヤーの回転処理
void Player::Rotation()
{
	CQuaternion qRot;
	//移動した向きに回転させる。
	m_Rot = atan2(m_moveSpeed.x, m_moveSpeed.z);
	qRot.SetRotation(CVector3::AxisY(), m_Rot);
	Gmodel.SetRotation(qRot);
	//もし、動いていたら回転させる。
	if (m_moveSpeed.x != None || m_moveSpeed.z != None)
	{
		m_rotation = qRot;
		Gmodel.SetRotation(m_rotation);
	}
	else if (m_moveSpeed.x == None || m_moveSpeed.z == None)
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
		g_anim.Play(pl_idle, 0.1f);
		StopSound(1);
		break;
	case pl_Walk:	//歩き状態。
		MoveOperation();
		g_anim.Play(pl_Walk, 0.1f);
		StopSound(1);
		break;
	case pl_FlyMove:
		MoveOperation();
		g_anim.Play(pl_FlyMove, 0.1f);
		SoundTrue(1);
		break;
	case pl_Atk:	//攻撃状態。
		ComboAttack();
		toEnemyInduction();				//追従
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
	if (m_isCombo_flag && g_pad[0].IsTrigger(enButtonY))
	{
		m_ComboNow_flag = true;
	}
	else if (g_pad[0].IsTrigger(enButtonY))
	{
		toEnemyInduction();				//追従
		playerState = pl_Atk;
		m_isCombo_flag = true;
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
			if (!g_pad[0].IsPress(enButtonA) || playerENE == ene_Charge)
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
		else if (g_pad[0].IsPress(enButtonX) && playerENE == ene_Full)
		{
			playerState = pl_FlyMove;
		}
		//歩きの時。
		else if (!g_pad[0].IsPress(enButtonX) || playerENE == ene_Charge)
		{
			playerState = pl_Walk;
		}
}
//アニメーションイベント
void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//ゴーストオブジェクトを作る位置を決めています。
	boxPos = m_position + (m_forward * UpPhyGhostObjPosition);
	boxPos.y += UpPhyGhostObjPosition;
	float m_efkPosUpY = 65.0f;
	//アニメーションイベントが呼ばれたときにゴーストオブジェクトを生成。
	m_PhyGhostObj.CreateBox(boxPos, m_rotation, box_scale);
	for (auto& listener : m_eventListenerList) {
		listener->OnOccurredAttackCollision(m_PhyGhostObj,eventName,ATK);
	}

	//削除。
	m_PhyGhostObj.Release();
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
	//リスナーにダメージを受けたことを通知する。
	for (auto& Listener : m_eventListenerList)
	{
		Listener->OnDamage(this);
	}
	//もし、HPが0以下なら死亡処理。
	if (HP <= 0.0f)
	{
		m_isdeath_flag = true;
	}
	//You are Dead。
	if (m_isdeath_flag)
	{
		playerState = pl_Death;
		//プレイヤーが死亡したことをイベントリスナーに通知。
		NotifyDeadEventToListener();
	}
}
//リスナーにプレイヤーが死んだことを通知する処理。
void Player::NotifyDeadEventToListener()
{
	//イベントリスナーに死亡を通知する。
	for (auto& listener : m_eventListenerList) {
		listener->OnPlayerDead(this);
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
			ENERGY -= ENERGYFLUCT/1.5f;
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
	//現在のロックオン状態を記録する。
	bool isOldLockOnFlag = m_isLockOn_flag;
	//プレイヤーの向いている角度の計算。
	float degreep = atan2(m_forward.x, m_forward.z);
	//計算して出た暫定的に一番小さい角度を記憶する変数。
	float degreemum = M_PI * 2;
	//範囲外。
	float distance = 2000.0f;
	//一番近いエネミーとの距離。初期値は大きな数値にしておく。
	float distanceNearEnemy = 10000000;
	//ロックオン可能フラグ
	bool isPossibleLockOn = false;
	for (Enemys* enemys : m_enemysList)
	{
		if (enemys->GetisDeath())
			continue;
		CVector3 dist = m_position - enemys->GetPosition();
		//①視点から注視点に向かって伸びるベクトルV1を計算する。
		CVector3 m_cameraToPlayer = g_Camera->GetPosition() - m_position;
		//②視点からエネミーに向かって伸びるベクトルV2を計算する。
		CVector3 m_cameraToEnemy = g_Camera->GetPosition() - enemys->GetPosition();
		//③V1を正規化する。
		m_cameraToPlayer.Normalize();
		//④V2を正規化する。
		m_cameraToEnemy.Normalize();
		//⑤V1とV2の内積結果tを求める。
		float t = m_cameraToPlayer.Dot(m_cameraToEnemy);


		float distEnemy = dist.Length();
		if (distEnemy < distanceNearEnemy
			&& distEnemy < distance
			&& t >= ZERO  
		) {
			//こいつのほうが近い。
			//一番近いエネミーの距離を更新する。
			distanceNearEnemy = distEnemy;
			m_enemyPos = enemys->GetPosition();
			//ロックオン可能にする。
			isPossibleLockOn = true;
		}

	}	
	//求めた一番小さい値が一定値より小さい場合、ターゲティングをオンにする。
	if (m_isLockOn_flag) {
		if (g_pad[0].IsTrigger(enButtonB)) {
			m_isLockOn_flag = false;
		}
	}
	else {
		if (isPossibleLockOn) {
			if (g_pad[0].IsTrigger(enButtonB)) {
				m_isLockOn_flag = true;
			}
		}
	}

	//ロックオンの切り替わりをリスナーに通知。
	TryNotifyChangeLockonEventToListener(isOldLockOnFlag);
}
//プレイヤーがカメラの注視点を変更したときに呼ぶ処理
void Player::TryNotifyChangeLockonEventToListener(bool isOldLockOnFlag)
{
	if (isOldLockOnFlag != m_isLockOn_flag) {
		//ロックオン状態が変わったので、リスナーに通知。
		if (m_isLockOn_flag) {
			//ロックオン状態に切り替わったのをリスナーに通知。
			for (auto& listener : m_eventListenerList) {
				listener->OnStartLockOn(this);
			}
		}
		else {
			//ロックオン状態が終わったのをリスナーに通知。
			for (auto& listener : m_eventListenerList) {
				listener->OnEndLockOn(this);
			}
		}
	}
}
//攻撃中に再度入力することで、2発目の攻撃がでます。
void Player::ComboAttack()
{
	//コンボじゃないときは一回
	if (!m_ComboNow_flag)
	{
		g_anim.Play(pl_Atk,0.1f);

		//攻撃が終わったら。
		if (!g_anim.IsPlaying())
		{
			ComboReset();
		}
	}
	//コンボ中は二回
	else if (m_ComboNow_flag&& !g_anim.IsPlaying())
	{
		g_anim.Play(pl_Combo,0.1f);
		//攻撃が終わったら。
		if (!g_anim.IsPlaying())
		{
			ComboReset();
		}
	}
}
//回復するときの記述
void Player::PlayerHeal(int healPoint)
{
	HP += healPoint;
	//HP上限が超えないように。
	if (HP > MaxHp)
	{
		HP = MaxHp;
	}
}
//敵の方向に向ける処理。
void Player::toEnemyInduction()
{
	float m_distance = 50.0f;
	//リストから敵全員の位置を使います。
	for (auto enemy : m_enemysList)
	{
		if (!enemy->GetIsDead()) {
			toEnemy = enemy->GetPosition() - m_position;
			Angle();
		}
		//距離内かつ角度内。
		if (toNearEnemyPos.Length() <= m_distance
			&& fabsf(m_angle) < CMath::PI * RookAngle)
		{
			//計算、代入。そしてモデルにセット。
			PlayerRot = atan2(toNearEnemyPos.x, toNearEnemyPos.z);
			m_rotation.SetRotation(CVector3::AxisY(), PlayerRot);
			Gmodel.SetRotation(m_rotation);
			isDiscoveryFlag = true;
		}
		else if(toNearEnemyPos.Length() >= m_distance
			&& fabsf(m_angle) > CMath::PI* RookAngle)
		{
			//範囲にいないならその場所で攻撃モーション。
			m_rotation.SetRotation(CVector3::AxisY(),m_Rot);
			Gmodel.SetRotation(m_rotation);
			isDiscoveryFlag = false;
		}
	}
}
//アングル。
void Player::Angle()
{
	//一番近い敵のポジションを計算。
	if (isDiscoveryFlag &&toEnemy.Length() < NearEnemys)
	{
		toNearEnemyPos = toEnemy;
	}
	toNearEnemyPos.Normalize();
	m_angle = acosf(toNearEnemyPos.Dot(m_forward));
}