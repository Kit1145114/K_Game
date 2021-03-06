#include "stdafx.h"
#include "ITEMBox.h"


ITEMBox::ITEMBox()
{
	
}

ITEMBox::~ITEMBox()
{
}

bool ITEMBox::Start()
{
	Box.Init(L"Assets/modelData/BOX.cmo");						//モデルの呼び出し。
	box_animClip[open].Load(L"Assets/animData/box_open.tka");	//開けのロード
	box_animClip[open].SetLoopFlag(false);						//箱開け。
	box_animClip[close].Load(L"Assets/animData/box_idle.tka");	//閉じてるロード
	box_animClip[close].SetLoopFlag(false);						//箱開け。
	box_anim.Init(
		Box,				//モデル
		box_animClip,		//アニメーションクリップ
		m_AnimClipNum		//アニメーションの数
	);
	box_anim.Play(close);
	state = close;
	m_openSound.Init(L"Assets/sound/enemy_attack_00.wav");
	m_charaCon.Init(100.0f, 50.0f, m_position, enCollisionAttr_Enemy);	//キャラコンの設定（半径、高さ、初期位置。）
	m_effect[0] = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/drop.efk");
	return true;
}
//アップデート処理。
void ITEMBox::Update()
{
	State();
	box_anim.Update(GameTime().GetFrameDeltaTime());
	Box.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());
}
//ドロー関数。
void ITEMBox::Draw()
{
	Box.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		1
	);
}
//ステート
void ITEMBox::State()
{
	switch (state)
	{
	case ITEMBox::open:
		box_anim.Play(open);
		Open();
		break;
	case ITEMBox::close:
		Close();
		box_anim.Play(close);
		break;
	case ITEMBox::death:
		Delete();
		break;
	}
}
//削除
void ITEMBox::Delete()
{
	g_goMgr.DeleteGO(this);
	m_player->DeleteEvenetListener(this);
}
//開けるかどうか。
void ITEMBox::Open()
{

	if (!box_anim.IsPlaying())
	{
		//アニメーションが終わったらアイテム生成。
		if (!isItemDrop_flag)
		{
			RItem = g_goMgr.NewGO<RecoveryITEM>();
			RItem->SetPositon(m_position);
			RItem->SetPlayer(m_player);
			isItemDrop_flag = true;
		}
		m_timer += GameTime().GetFrameDeltaTime();
		//時間で削除。
		if (m_timer > m_deathTime) {
			state = death;
		}
	}
}

void ITEMBox::Close()
{
	if (isOpen_flag)
	{
		state = open;
		m_playEffectHandle = g_effektEngine->Play(m_effect[0]);
		g_effektEngine->SetPosition(m_playEffectHandle, m_position);
	}
}

void ITEMBox::OnOccurredAttackCollision(PhysicsGhostObject& colli, const wchar_t* eventName, int m_playerAtkPoint)
{
	g_physics.ContactTest(m_charaCon, [&](const btCollisionObject& contactObject) {
		if (colli.IsSelf(contactObject) == true && eventName) {
			SetIsOpen(true);
			m_openSound.Play(0);						//音;
		}
	});
}

void ITEMBox::BindPlayer(Player* player)
{
	m_player = player;
	player->AddEventListener(this);
}