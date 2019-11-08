#include "stdafx.h"
#include "Sample.h"


Sample::Sample()
{
	SModel.Init(L"Assets/modelData/enemy_00.cmo");
	s_animClip[0].Load(L"Assets/animData/idle.tka");
	s_animClip[0].SetLoopFlag(true);
	s_animClip[1].Load(L"Assets/animData/attack.tka");
	s_animClip[1].SetLoopFlag(true);
	s_anim.Init(
		SModel,
		s_animClip,
		3
	);
	s_anim.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName)
	{
		OnAnimationEvent(clipName, eventName);
	});
	prm.HP = 100;										//HP
	prm.ATK = 60;										//攻撃力
	prm.DEF = 30;										//防御力
	prm.SPD = 10;										//速さ。
	m_position = e4_pos;
	s_state = SSIdle;
	m_charaCon.Init(150.0f, 10.0f, m_position);
}
void Sample::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	if (eventName)
	{
		
	}
	//auto ss = NewGO<prefab::CSoundSource>(0);
	//ss->Init(eventName);%
	//ss->SetVolume(4.0f);
	//ss->Play(false);
}
void Sample::Update()
{
	Draw();
	State();
	Track();
	s_anim.Update(1.0f / 30.0f);
	m_charaCon.SetPosition(m_position);
	SModel.UpdateWorldMatrix(m_position, CQuaternion::Identity(), m_scale);
}

void Sample::Draw()
{
	SModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void Sample::Damage(float Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//もし、HPが0以下なら死亡処理。
	if (prm.HP <= 0.0f)
	{
		Death();
	}
}
//敵の状態。
void Sample::State() {
	switch (s_state)
	{
	case Sample::SSIdle:
		m_moveSpeed.y -= 100.0f; //* (1.0f / 60.0f);
		m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
		s_anim.Play(0);
		break;
	case Sample::SSAttack:
		m_moveSpeed.y -= 100.0f; //* (1.0f / 60.0f);
		m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
		s_anim.Play(1);
		break;
	default:
		break;
	}
}
//プレイヤーを見つけたかの処理。
void Sample::Track()
{
	CVector3 diff = m_position - m_player->GetPosition();
	if (diff.Length() <= 200.0f)
	{
		s_state = SSAttack;
	}
	else
	{
		s_state = SSIdle;
	}
}
