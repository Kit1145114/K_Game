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
	m_HP		=	HP;				//�q�b�g�|�C���g����
	m_ATK		=	Attack;			//�U���͂����B
	m_DEF		=	Defense;		//�h��͂����B
	m_SPD		=	Speed;			//��b���x�����B
	//m_sm		=	model;			//�G�l�~�[�̃��f���B
}
void Enemys::BindPlayer(Player* player)
{
	m_player = player;
	player->AddEventListener(this);
}
//���ʂ̃h���[�֐��Ȃ̂ł����ɋL�ځB
void Enemys::Draw()
{
	Model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		1
	);
}
//���ʂ̎���p
void Enemys::ViewingAngle()
{
	m_diff = m_player->GetPosition() - m_position;
	m_toPlayer = m_player->GetPosition() - m_position;
	m_toPlayer.Normalize();
	m_angle = acosf(m_toPlayer.Dot(m_forward));
}
//���ʂ̃x�N�g���̎擾�����B
void Enemys::VectorAcquisition()
{
	auto mRot = CMatrix::Identity();
	mRot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = mRot.m[2][0];
	m_forward.y = mRot.m[2][1];
	m_forward.z = mRot.m[2][2];
}
//���ʂ̉�]�����B
void Enemys::Rotation()
{
	float Rot = atan2(m_move.x, m_move.z);
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisY(), Rot);
	Model.SetRotation(qRot);
	//�����A�����Ă������]������B
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
//�U���̃G�t�F�N�g�B�i���n�Ɏg���܂��B
void Enemys::EnemyEffect()
{
	//�G�t�F�N�g�̈ʒu�𒲐��B
	m_efePos = (m_player->GetPosition() + m_position) * 0.5f;
	m_efeRot = m_player->GetPosition() - m_position;
	//������ɃG�t�F�N�g��\���������̂ŁB
	m_efePos.y += 30.0f;
	m_playEffectHandle = g_effektEngine->Play(m_attackEffect);
	g_effektEngine->SetPosition(m_playEffectHandle, m_efePos);
	g_effektEngine->SetRotation(m_playEffectHandle, 0.0f, atan2(m_efeRot.x, m_efeRot.z), 0.0f);
}
//�U���󂯂��Ƃ����Ⴂ�B
void Enemys::OnOccurredAttackCollision(PhysicsGhostObject& colli, const wchar_t* eventName, int m_playerAtkPoint)
{
	//�G�̃I�u�W�F�N�g�ƁA�S�[�X�g���Ԃ����Ă��邩�B
	g_physics.ContactTest(m_charaCon, [&](const btCollisionObject& contactObject) {
		//�Ԃ����āA�A�j���[�V�����C�x���g���Ă΂ꂽ��B
		if (colli.IsSelf(contactObject) == true && eventName) {
			//�_���[�W�A���B
			Damage(m_playerAtkPoint);
			SetHitMe(true);
			m_damagedSe[0];
			//�G�l�~�[�Ƃ̊ԂɃG�t�F�N�g�𔭐������邽�߂�
			//�v���C���[�ƓG�̊Ԃ��v�Z���Ă��܂��B
			CVector3 m_playerDistance = m_position; //= (m_position + enemy->GetPosition()) / 2;
			m_playerDistance.y += m_efkPosUpY;
			m_playEffectHandle = g_effektEngine->Play(m_damagedEffect);
			g_effektEngine->SetPosition(m_playEffectHandle, m_playerDistance);
		}
		//�f�U�肾�����ꍇ�A�ʂ̉������炷�B
		else if (colli.IsSelf(contactObject) == false && eventName)
		{
			m_damagedSe[1];
		}
	});
}
