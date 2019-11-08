/*!
 *@brief	�A�j���[�V�����N���b�v�B
 */

#pragma once

//////////////////////////////////////////////////////////////////////
// �e��\����
//////////////////////////////////////////////////////////////////////
/*!
*@brief	�A�j���[�V�����N���b�v�̃w�b�_�[�B
*/
struct AnimClipHeader {
	std::uint32_t numKey;				//!<�L�[�t���[���̐��B
	std::uint32_t numAnimationEvent;	//!<�A�j���[�V�����C�x���g�̐��B
};
/*!
*@brief	�A�j���[�V�����C�x���g
*@�A�j���[�V�����C�x���g�͖��Ή��B
* ��肽�������玩���Ŏ�������悤�ɁB
*/
struct AnimationEventData {
	float	invokeTime;					//!<�A�j���[�V�����C�x���g���������鎞��(�P��:�b)
	std::uint32_t eventNameLength;		//!<�C�x���g���̒����B
};
/*!
*@brief	�L�[�t���[���B
*/
struct Keyframe {
	std::uint32_t boneIndex;	//!<�{�[���C���f�b�N�X�B
	float time;					//!<���ԁB
	CMatrix transform;			//!<�g�����X�t�H�[���B
};
/*!
*@brief	�L�[�t���[���B
*/
struct KeyframeRow {
	std::uint32_t boneIndex;	//!<�{�[���C���f�b�N�X�B
	float time;					//!<���ԁB
	CVector3 transform[4];		//!<�g�����X�t�H�[���B
};
/// <summary>
/// �A�j�C���[�V�����C�x���g
/// </summary>
struct CAnimationEvent {
	float	invokeTime;					//!<�A�j���[�V�����C�x���g���������鎞��(�P��:�b)
	std::uint32_t eventNameLength;		//!<�C�x���g���̒����B
};
class AnimationEvent
{
public:
	AnimationEvent(){};
	/// <summary>
	/// �C�x���g�������Ԃ�ݒ�B
	/// </summary> 
	float GetInvokeTime() const
	{
		return m_invokeTime;
	}
	/// <summary>
	/// �C�x���g�����擾�B
	/// </summary>
	const wchar_t* GetEventName()const
	{
		return m_eventName.c_str();
	}
	/// <summary>
	/// �C�x���g�̔������Ԃ�ݒ�B
	/// </summary>
	void SetInvokeTime(float time)
	{
		m_invokeTime = time;
	}
	/// <summary>
	/// �C�x���g����ݒ�B
	/// </summary>
	void SetEventName(const wchar_t* name)
	{
		m_eventName = name;
	}
	/// <summary>
	/// �C�x���g�������ς݂�����B
	/// </summary>
	bool IsInvoked()const
	{
		return m_isInvoked;
	}
	/// <summary>
	/// �C�x���g�������ς݂̃t���O��ݒ肷��B
	/// </summary>
	void SetInvokedFlag(bool flag)
	{
		m_isInvoked = flag;
	}
private:
	bool m_isInvoked = false;	//�C�x���g�������ς݂�
	float m_invokeTime;			//�C�x���g��������
	std::wstring m_eventName;	//�C�x���g�̖��O
};
/*!
*@brief	�A�j���[�V�����N���b�v�B
*/
class AnimationClip  : public AnimationEvent{
public:
	struct SFreezeBoneInfo {
		int boneNo;		//!<�t���[�Y������{�[���ԍ��B
		bool freezeX;	//!<X�����̈ړ����t���[�Y������H
		bool freezeY;	//!<Y�����̈ړ����t���[�Y������H
		bool freezeZ;	//!<Z�����̈ړ����t���[�Y������H
	};
	//�^�C�v�ʂ��������炵���āA�������̂�std::vector<KeyFrame*>�̕ʖ���`�B
	using keyFramePtrList = std::vector<Keyframe*>;
	/*!
	* @brief	�R���X�g���N�^
	*/
	AnimationClip() 
	{
	}
	/*!
	*@brief	�f�X�g���N�^�B
	*/
	~AnimationClip();
	/*!
	*@brief	�A�j���[�V�����N���b�v�����[�h�B
	*@param[in]	filePath	�t�@�C���p�X�B
	*/
	void Load(const wchar_t* filePath);

	/*!
	*@brief	���[�v����H
	*/
	bool IsLoop() const
	{
		return m_isLoop;
	}
	/*!
	*@brief	���[�v�t���O��ݒ肷��B
	*/
	void SetLoopFlag(bool flag)
	{
		m_isLoop = flag;
	}
	/*!
	*@brief
	*/
	const std::vector<keyFramePtrList>& GetKeyFramePtrListArray() const
	{
		return m_keyFramePtrListArray;
	}
	const keyFramePtrList& GetTopBoneKeyFrameList() const
	{
		return *m_topBoneKeyFramList;
	}
	//��������A�j���[�V�����C�x���g�p�ɍ�����֐��B
	/// <summary>
	/// �A�j���[�V�����C�x���g���擾�B
	/// </summary>
	std::unique_ptr<AnimationEvent[]>& GetAnimationEvent()
	{
		return m_animationEvent;
	}
	/// <summary>
	/// �A�j���[�V�����C�x���g�̐����擾�B
	/// </summary>
	int GetNumAnimationEvent() const
	{
		return m_numAnimationEvent;
	}
	/// <summary>
	/// �A�j���[�V�����N���b�v�̓��[�h�ς݂��ǂ����B
	/// </summary>
	bool IsLoaded() const
	{
		return m_loaded;
	}
	//�N���b�v�����擾
	const wchar_t* GetName() const
	{
		return m_clipName.c_str();
	}
	/*const GetFreezeBoneInfo*/
private:
	bool m_isLoop = false;										//!<���[�v�t���O�B
	std::vector<Keyframe*> m_keyframes;							//�S�ẴL�[�t���[���B
	std::vector<keyFramePtrList> m_keyFramePtrListArray;		//�{�[�����Ƃ̃L�[�t���[���̃��X�g���Ǘ����邽�߂̔z��B
																//�Ⴆ�΁Am_keyFramePtrListArray[0]��0�Ԗڂ̃{�[���̃L�[�t���[���̃��X�g�A
	std::wstring m_clipName;															//m_keyFramePtrListArray[1]��1�Ԗڂ̃{�[���̃L�[�t���[���̃��X�g�Ƃ����������B
	std::unique_ptr<AnimationEvent[]>	m_animationEvent;		//�A�j���[�V�����C�x���g�B
	int									m_numAnimationEvent = 0;	//�A�j���[�V�����C�x���g�̐��B
	using KeyframePtr = std::unique_ptr<Keyframe>;
	keyFramePtrList* m_topBoneKeyFramList = nullptr;
	SFreezeBoneInfo m_freezeBoneInfo;	//!<�t���[�Y������{�[�����B
	bool m_loaded = false;	//�A�j���[�V�����N���b�v�����[�h����Ă���H
};
using CAnimationClipPtr = std::unique_ptr<AnimationClip>;

