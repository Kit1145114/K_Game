#pragma once
//�Q�[�������ʕ����������ɋL�����B

//�O���[�o���萔

const float gravity = 20.0f;							//�d�́B
const float UpPhyGhostObjPosition = 200.0f;				//�S�[�X�g�I�u�W�F�N�g�̐����ʒu�����߂�B
const int m_AnimClipNum = 10;							//�A�j���[�V�����N���b�v�̐��w��B
const int ENAMY_NUM = 3;								//�G�𐶐����鐔�������Ō��߂�B
const int ZERO = 0;
const CVector3 e_pos1 = { 400.0f,-50.0f,0.0f };			//�G�P�̃|�W�V�����B
const CVector3 e_pos2 = { 0.0f,-50.0f,0.0f };			//�G�Q�̃|�W�V�����B
const CVector3 e_pos3 = { -400.0f,-50.0f,0.0f };		//�G�R�̃|�W�V�����B
const CVector2 PIVOT = { 0.0f,0.5f };					//�s�{�b�g�B
const float	 SPRITERE_SOLUTION_W = 1280.0f;				//�摜��
const float	 SPRITERE_SOLUTION_H = 720.0f;				//�摜�c
	//�X�e�[�W�̐U�蕪���B
const enum StageNum
{
	First,	//�����X�e�[�W
	Second	//�{�X�X�e�[�W
};