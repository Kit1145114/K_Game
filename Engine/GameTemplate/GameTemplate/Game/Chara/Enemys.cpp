#include "stdafx.h"
#include "Enemys.h"
#include"StoneEnemy.h"

Enemys::Enemys()
{
	
}

Enemys::~Enemys()
{

}

void Enemys::Init(float HP, float Attack, float Defense, float Speed/*, SkinModel model*/)
{
	m_HP		=	HP;				//�q�b�g�|�C���g����
	m_ATK		=	Attack;			//�U���͂����B
	m_DEF		=	Defense;		//�h��͂����B
	m_SPD		=	Speed;			//��b���x�����B
	//m_sm		=	model;			//�G�l�~�[�̃��f���B
}