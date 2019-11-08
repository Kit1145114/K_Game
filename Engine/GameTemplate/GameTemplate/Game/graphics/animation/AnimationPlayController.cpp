/*!
 * @brief	�A�j���[�V�����Đ��R���g���[���B
 */

#include "stdafx.h"
#include "graphics/animation/AnimationPlayController.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "graphics/skeleton.h"

//���A�j���[�V�����C�x���g�p
//�A�j���[�V�����C�x���g�N��
void AnimationPlayController::InvokeAnimationEvent(Animation* animation)
{
	auto& animEventArray = m_animationClip->GetAnimationEvent();
	for (auto i = 0; i < m_animationClip->GetNumAnimationEvent(); i++) {
		if (m_time > animEventArray[i].GetInvokeTime()
			&& animEventArray[i].IsInvoked() == false) {
			//�A�j���[�V�����̋N�����Ԃ��߂��Ă��銎�A�܂��C�x���g�N�����Ă��Ȃ��B
			animation->NotifyAnimationEventToListener(
				m_animationClip->GetName(), animEventArray[i].GetEventName()
			);
			animEventArray[i].SetInvokedFlag(true);
		}
	}
}
//���[�v�Đ��J�n�B
void AnimationPlayController::StartLoop()
{
	m_currentKeyFrameNo = 0;
	m_time = 0.0f;
	//�A�j���[�V�����C�x���g�����ׂĖ������ɂ���B
	auto& animEventArray = m_animationClip->GetAnimationEvent();
	for (auto i = 0; i < m_animationClip->GetNumAnimationEvent(); i++) {
		animEventArray[i].SetInvokedFlag(false);
	}
}
//�w�肳�ꂽ�{�[���̃A�j���[�V�����̃t���[�Y
void AnimationPlayController::ExecuteFreezeBone()
{
	////�t���[�Y������{�[���ԍ����擾�B
	//const AnimationClip::SFreezeBoneInfo& freezeBoneInfo = m_animationClip->GetFreezeBoneInfo();

	//if (freezeBoneInfo.boneNo == -1) {
	//	//�t���[�Y������{�[�����Ȃ��B
	//	return;
	//}

	//for (int boneNo = 0; boneNo < m_skeleton->GetNumBones(); boneNo++) {
	//	//�ړI�̃{�[���̃��[���h�s������߂�B
	//	CBone* bone = m_skeleton->GetBone(boneNo);
	//	if (bone->GetParentId() == -1) {
	//		//���[�g�{�[�����������B
	//		CMatrix freezeBoneWorldMatrix;
	//		CalcFreezeBoneWorldMatrix(*bone, CMatrix::Identity, freezeBoneWorldMatrix);
	//		m_freezeBoneTranslate = CVector3::Zero;
	//		if (freezeBoneInfo.freezeX) {
	//			//X���̈ړ����t���[�Y�B
	//			m_freezeBoneTranslate.x = freezeBoneWorldMatrix.m[3][0];
	//			m_boneMatrix[bone->GetNo()].m[3][0] -= freezeBoneWorldMatrix.m[3][0];
	//		}
	//		if (freezeBoneInfo.freezeY) {
	//			//Y���̈ړ����t���[�Y�B
	//			m_freezeBoneTranslate.y = freezeBoneWorldMatrix.m[3][1];
	//			m_boneMatrix[bone->GetNo()].m[3][1] -= freezeBoneWorldMatrix.m[3][1];
	//		}
	//		if (freezeBoneInfo.freezeZ) {
	//			//Z���̈ړ����t���[�Y�B
	//			m_freezeBoneTranslate.z = freezeBoneWorldMatrix.m[3][2];
	//			m_boneMatrix[bone->GetNo()].m[3][2] -= freezeBoneWorldMatrix.m[3][2];
	//		}
	//		break;
	//	}
	//}
}
//�t���[�Y�����ځ[��̃��[���h�s������߂�B
void AnimationPlayController::CalcFreezeBoneWorldMatrix(Bone& bone, const CMatrix& parentMatrix, CMatrix& freezeBoneMatrix)
{

}
//���A�j���[�V�����B	
void AnimationPlayController::Init(Skeleton* skeleton)
{
	int numBones = skeleton->GetNumBones();
	//�{�[���s����o�V�b�Ɗm�ہB
	m_boneMatrix.resize(numBones);
	//m_skeleton = skeleton;
}
	
void AnimationPlayController::Update(float deltaTime, Animation* animation)
{
	if(m_animationClip == nullptr){
		
		return ;
	}
	const auto& topBoneKeyFrameList = m_animationClip->GetTopBoneKeyFrameList();
	m_time += deltaTime;
	InvokeAnimationEvent(animation);
	//�⊮���Ԃ��i�߂Ă����B
	m_interpolateTime = min(1.0f, m_interpolateTime + deltaTime);
	while (true) {
		if (m_currentKeyFrameNo >= (int)topBoneKeyFrameList.size()) {
			//�I�[�܂ōs�����B
			if (m_animationClip->IsLoop()) {
				//���[�v�B
				StartLoop();
			}
			else {
				//�����V���b�g�Đ��B
				m_currentKeyFrameNo--;
				m_isPlaying = false;	//�Đ��I���B
			}
			break;
		}
		if (topBoneKeyFrameList.at(m_currentKeyFrameNo)->time >= m_time) {
			//�I���B
			break;
		}
		//���ցB
		m_currentKeyFrameNo++;
	}
	//�{�[���s����v�Z���Ă����B
	const auto& keyFramePtrListArray = m_animationClip->GetKeyFramePtrListArray();
	for (const auto& keyFrameList : keyFramePtrListArray) {
		if (keyFrameList.size() == 0) {
			continue;
		}
		//���ݍĐ����̃L�[�t���[��������Ă���B
		Keyframe* keyframe = keyFrameList.at(m_currentKeyFrameNo);
		if (keyframe->boneIndex < m_boneMatrix.size()) {
			m_boneMatrix[keyframe->boneIndex] = keyframe->transform;
		}
		else {
#ifdef _DEBUG			
			MessageBox(NULL, "AnimationPlayController::Update : ���݂��Ȃ��{�[���ɒl�������������Ƃ��Ă��܂��B���̂悤�Ȍ������l�����܂��B\n"
				"�@ tka�t�@�C�����o�͂��鎞�ɁA�I���������[�g�{�[�����X�P���g���̃��[�g�{�[���ƈقȂ��Ă���B\n"
				"�A �قȂ�X�P���g���̃A�j���[�V�����N���b�v���g���Ă���B\n"
				"������xtka�t�@�C�����o�͂��Ȃ����Ă��������B", "error", MB_OK);
			std::abort();
#endif
		}
	}
}

