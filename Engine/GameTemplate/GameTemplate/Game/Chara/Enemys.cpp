#include "stdafx.h"
#include "Enemys.h"

Enemys::Enemys()
{
	m_damagedEffect = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/hit.efk");
	m_se[0].Init(L"Assets/sound/enemy_attack_00.wav");
	m_se[1].Init(L"Assets/sound/swing.wav");
}

Enemys::~Enemys()
{
	m_player->DeleteEvenetListener(this);
}

void Enemys::Init(float HP, float Attack, float Defense, float Speed/*, SkinModel model*/)
{
	m_HP		=	HP;				//ヒットポイントを代入
	m_ATK		=	Attack;			//攻撃力を代入。
	m_DEF		=	Defense;		//防御力を代入。
	m_SPD		=	Speed;			//基礎速度を代入。
	//m_sm		=	model;			//エネミーのモデル。
}
void Enemys::BindPlayer(Player* player)
{
	m_player = player;
	player->AddEventListener(this);
}
//共通のドロー関数なのでここに記載。
void Enemys::Draw()
{
	Model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		1
	);
}
//共通の視野角
void Enemys::ViewingAngle()
{
	m_diff = m_player->GetPosition() - m_position;
	m_toPlayer = m_player->GetPosition() - m_position;
	m_toPlayer.Normalize();
	m_angle = acosf(m_toPlayer.Dot(m_forward));
}
//共通のベクトルの取得処理。
void Enemys::VectorAcquisition()
{
	auto mRot = CMatrix::Identity();
	mRot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = mRot.m[2][0];
	m_forward.y = mRot.m[2][1];
	m_forward.z = mRot.m[2][2];
}
//共通の回転処理。
void Enemys::Rotation()
{
	float Rot = atan2(m_move.x, m_move.z);
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisY(), Rot);
	Model.SetRotation(qRot);
	//もし、動いていたら回転させる。
	if (m_moveSpeed.x != ZERO || m_moveSpeed.z != ZERO)
	{
		m_rotation = qRot;
		Model.SetRotation(m_rotation);
	}
	if (m_moveSpeed.x == ZERO && m_moveSpeed.z == ZERO)
	{
		Model.SetRotation(m_rotation);
	}
	Model.SetRotation(m_rotation);
}
//攻撃のエフェクト。格闘系に使います。
void Enemys::EnemyEffect()
{
	//エフェクトの位置を調整。
	m_efePos = (m_player->GetPosition() + m_position) * 0.5f;
	m_efeRot = m_player->GetPosition() - m_position;
	//少し上にエフェクトを表示したいので。
	m_efePos.y += 30.0f;
	m_playEffectHandle = g_effektEngine->Play(m_attackEffect);
	g_effektEngine->SetPosition(m_playEffectHandle, m_efePos);
	g_effektEngine->SetRotation(m_playEffectHandle, 0.0f, atan2(m_efeRot.x, m_efeRot.z), 0.0f);
}
//攻撃受けたときじゃい。
void Enemys::OnOccurredAttackCollision(PhysicsGhostObject& colli, const wchar_t* eventName, int m_playerAtkPoint)
{
	//敵のオブジェクトと、ゴーストがぶつかっているか。
	g_physics.ContactTest(m_charaCon, [&](const btCollisionObject& contactObject) {
		//ぶつかって、アニメーションイベントが呼ばれたら。
		if (colli.IsSelf(contactObject) == true && eventName) {
			//ダメージ、音。
			Damage(m_playerAtkPoint);
			SetHitMe(true);
			m_damagedSe[0];
			//エネミーとの間にエフェクトを発生させるために
			//プレイヤーと敵の間を計算しています。
			CVector3 m_playerDistance = m_position; //= (m_position + enemy->GetPosition()) / 2;
			m_playerDistance.y += m_efkPosUpY;
			m_playEffectHandle = g_effektEngine->Play(m_damagedEffect);
			g_effektEngine->SetPosition(m_playEffectHandle, m_playerDistance);
		}
		//素振りだった場合、別の音だけ鳴らす。
		else if (colli.IsSelf(contactObject) == false && eventName)
		{
			m_damagedSe[1];
		}
	});
}
