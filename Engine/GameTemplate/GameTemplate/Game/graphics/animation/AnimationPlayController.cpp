/*!
 * @brief	アニメーション再生コントローラ。
 */

#include "stdafx.h"
#include "graphics/animation/AnimationPlayController.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "graphics/skeleton.h"

//※アニメーションイベント用
//アニメーションイベント起動
void AnimationPlayController::InvokeAnimationEvent(Animation* animation)
{
	auto& animEventArray = m_animationClip->GetAnimationEvent();
	for (auto i = 0; i < m_animationClip->GetNumAnimationEvent(); i++) {
		if (m_time > animEventArray[i].GetInvokeTime()
			&& animEventArray[i].IsInvoked() == false) {
			//アニメーションの起動時間を過ぎている且つ、まだイベント起動していない。
			animation->NotifyAnimationEventToListener(
				m_animationClip->GetName(), animEventArray[i].GetEventName()
			);
			animEventArray[i].SetInvokedFlag(true);
		}
	}
}
//ループ再生開始。
void AnimationPlayController::StartLoop()
{
	m_currentKeyFrameNo = 0;
	m_time = 0.0f;
	//アニメーションイベントをすべて未発生にする。
	auto& animEventArray = m_animationClip->GetAnimationEvent();
	for (auto i = 0; i < m_animationClip->GetNumAnimationEvent(); i++) {
		animEventArray[i].SetInvokedFlag(false);
	}
}
//指定されたボーンのアニメーションのフリーズ
void AnimationPlayController::ExecuteFreezeBone()
{
	////フリーズさせるボーン番号を取得。
	//const AnimationClip::SFreezeBoneInfo& freezeBoneInfo = m_animationClip->GetFreezeBoneInfo();

	//if (freezeBoneInfo.boneNo == -1) {
	//	//フリーズさせるボーンがない。
	//	return;
	//}

	//for (int boneNo = 0; boneNo < m_skeleton->GetNumBones(); boneNo++) {
	//	//目的のボーンのワールド行列を求める。
	//	CBone* bone = m_skeleton->GetBone(boneNo);
	//	if (bone->GetParentId() == -1) {
	//		//ルートボーンを見つけた。
	//		CMatrix freezeBoneWorldMatrix;
	//		CalcFreezeBoneWorldMatrix(*bone, CMatrix::Identity, freezeBoneWorldMatrix);
	//		m_freezeBoneTranslate = CVector3::Zero;
	//		if (freezeBoneInfo.freezeX) {
	//			//X軸の移動をフリーズ。
	//			m_freezeBoneTranslate.x = freezeBoneWorldMatrix.m[3][0];
	//			m_boneMatrix[bone->GetNo()].m[3][0] -= freezeBoneWorldMatrix.m[3][0];
	//		}
	//		if (freezeBoneInfo.freezeY) {
	//			//Y軸の移動をフリーズ。
	//			m_freezeBoneTranslate.y = freezeBoneWorldMatrix.m[3][1];
	//			m_boneMatrix[bone->GetNo()].m[3][1] -= freezeBoneWorldMatrix.m[3][1];
	//		}
	//		if (freezeBoneInfo.freezeZ) {
	//			//Z軸の移動をフリーズ。
	//			m_freezeBoneTranslate.z = freezeBoneWorldMatrix.m[3][2];
	//			m_boneMatrix[bone->GetNo()].m[3][2] -= freezeBoneWorldMatrix.m[3][2];
	//		}
	//		break;
	//	}
	//}
}
//フリーズしたぼーんのワールド行列を求める。
void AnimationPlayController::CalcFreezeBoneWorldMatrix(Bone& bone, const CMatrix& parentMatrix, CMatrix& freezeBoneMatrix)
{

}
//※アニメーション。	
void AnimationPlayController::Init(Skeleton* skeleton)
{
	int numBones = skeleton->GetNumBones();
	//ボーン行列をバシッと確保。
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
	//補完時間も進めていく。
	m_interpolateTime = min(1.0f, m_interpolateTime + deltaTime);
	while (true) {
		if (m_currentKeyFrameNo >= (int)topBoneKeyFrameList.size()) {
			//終端まで行った。
			if (m_animationClip->IsLoop()) {
				//ループ。
				StartLoop();
			}
			else {
				//ワンショット再生。
				m_currentKeyFrameNo--;
				m_isPlaying = false;	//再生終わり。
			}
			break;
		}
		if (topBoneKeyFrameList.at(m_currentKeyFrameNo)->time >= m_time) {
			//終わり。
			break;
		}
		//次へ。
		m_currentKeyFrameNo++;
	}
	//ボーン行列を計算していく。
	const auto& keyFramePtrListArray = m_animationClip->GetKeyFramePtrListArray();
	for (const auto& keyFrameList : keyFramePtrListArray) {
		if (keyFrameList.size() == 0) {
			continue;
		}
		//現在再生中のキーフレームを取ってくる。
		Keyframe* keyframe = keyFrameList.at(m_currentKeyFrameNo);
		if (keyframe->boneIndex < m_boneMatrix.size()) {
			m_boneMatrix[keyframe->boneIndex] = keyframe->transform;
		}
		else {
#ifdef _DEBUG			
			MessageBox(NULL, "AnimationPlayController::Update : 存在しないボーンに値を書き込もうとしています。次のような原因が考えられます。\n"
				"① tkaファイルを出力する時に、選択したルートボーンがスケルトンのルートボーンと異なっている。\n"
				"② 異なるスケルトンのアニメーションクリップを使っている。\n"
				"もう一度tkaファイルを出力しなおしてください。", "error", MB_OK);
			std::abort();
#endif
		}
	}
}

