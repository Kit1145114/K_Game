#pragma once
//�Q�[�����Ńv���C���[�ƓG�̋��ʕ����������ɋL�����B

//�O���[�o���萔
const float gravity = 20.0f;						//�d�́B
const float UpPhyGhostObjPosition = 150.0f;			//�S�[�X�g�I�u�W�F�N�g�̐����ʒu�����߂�B
const int m_AnimClipNum = 10;						//�A�j���[�V�����N���b�v�̐��w��B
const int ENAMY_NUM = 3;							//�G�𐶐����鐔�������Ō��߂�B
const int ZERO = 0;
const CVector3 e_pos1 = { 400.0f,-50.0f,0.0f };		//�G�P�̃|�W�V�����B
const CVector3 e_pos2 = { 0.0f,-50.0f,0.0f };		//�G�Q�̃|�W�V�����B
const CVector3 e_pos3 = { -400.0f,-50.0f,0.0f };	//�G�R�̃|�W�V�����B
const CVector2 PIVOT = { 0.0f,0.5f };				//�s�{�b�g�B
const enum StageNum
{
	FirstStage,	//�����X�e�[�W
	SecondStage	//�{�X�X�e�[�W
};