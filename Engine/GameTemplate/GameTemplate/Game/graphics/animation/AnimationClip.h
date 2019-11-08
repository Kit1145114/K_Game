/*!
 *@brief	アニメーションクリップ。
 */

#pragma once

//////////////////////////////////////////////////////////////////////
// 各種構造体
//////////////////////////////////////////////////////////////////////
/*!
*@brief	アニメーションクリップのヘッダー。
*/
struct AnimClipHeader {
	std::uint32_t numKey;				//!<キーフレームの数。
	std::uint32_t numAnimationEvent;	//!<アニメーションイベントの数。
};
/*!
*@brief	アニメーションイベント
*@アニメーションイベントは未対応。
* やりたかったら自分で実装するように。
*/
struct AnimationEventData {
	float	invokeTime;					//!<アニメーションイベントが発生する時間(単位:秒)
	std::uint32_t eventNameLength;		//!<イベント名の長さ。
};
/*!
*@brief	キーフレーム。
*/
struct Keyframe {
	std::uint32_t boneIndex;	//!<ボーンインデックス。
	float time;					//!<時間。
	CMatrix transform;			//!<トランスフォーム。
};
/*!
*@brief	キーフレーム。
*/
struct KeyframeRow {
	std::uint32_t boneIndex;	//!<ボーンインデックス。
	float time;					//!<時間。
	CVector3 transform[4];		//!<トランスフォーム。
};
/// <summary>
/// アニイメーションイベント
/// </summary>
struct CAnimationEvent {
	float	invokeTime;					//!<アニメーションイベントが発生する時間(単位:秒)
	std::uint32_t eventNameLength;		//!<イベント名の長さ。
};
class AnimationEvent
{
public:
	AnimationEvent(){};
	/// <summary>
	/// イベント発生時間を設定。
	/// </summary> 
	float GetInvokeTime() const
	{
		return m_invokeTime;
	}
	/// <summary>
	/// イベント名を取得。
	/// </summary>
	const wchar_t* GetEventName()const
	{
		return m_eventName.c_str();
	}
	/// <summary>
	/// イベントの発生時間を設定。
	/// </summary>
	void SetInvokeTime(float time)
	{
		m_invokeTime = time;
	}
	/// <summary>
	/// イベント名を設定。
	/// </summary>
	void SetEventName(const wchar_t* name)
	{
		m_eventName = name;
	}
	/// <summary>
	/// イベントが発生済みか判定。
	/// </summary>
	bool IsInvoked()const
	{
		return m_isInvoked;
	}
	/// <summary>
	/// イベントが発生済みのフラグを設定する。
	/// </summary>
	void SetInvokedFlag(bool flag)
	{
		m_isInvoked = flag;
	}
private:
	bool m_isInvoked = false;	//イベントが発生済みか
	float m_invokeTime;			//イベント発生時間
	std::wstring m_eventName;	//イベントの名前
};
/*!
*@brief	アニメーションクリップ。
*/
class AnimationClip  : public AnimationEvent{
public:
	struct SFreezeBoneInfo {
		int boneNo;		//!<フリーズさせるボーン番号。
		bool freezeX;	//!<X方向の移動をフリーズさせる？
		bool freezeY;	//!<Y方向の移動をフリーズさせる？
		bool freezeZ;	//!<Z方向の移動をフリーズさせる？
	};
	//タイプ量が長ったらしくて、うざいのでstd::vector<KeyFrame*>の別名定義。
	using keyFramePtrList = std::vector<Keyframe*>;
	/*!
	* @brief	コンストラクタ
	*/
	AnimationClip() 
	{
	}
	/*!
	*@brief	デストラクタ。
	*/
	~AnimationClip();
	/*!
	*@brief	アニメーションクリップをロード。
	*@param[in]	filePath	ファイルパス。
	*/
	void Load(const wchar_t* filePath);

	/*!
	*@brief	ループする？
	*/
	bool IsLoop() const
	{
		return m_isLoop;
	}
	/*!
	*@brief	ループフラグを設定する。
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
	//ここからアニメーションイベント用に作った関数。
	/// <summary>
	/// アニメーションイベントを取得。
	/// </summary>
	std::unique_ptr<AnimationEvent[]>& GetAnimationEvent()
	{
		return m_animationEvent;
	}
	/// <summary>
	/// アニメーションイベントの数を取得。
	/// </summary>
	int GetNumAnimationEvent() const
	{
		return m_numAnimationEvent;
	}
	/// <summary>
	/// アニメーションクリップはロード済みかどうか。
	/// </summary>
	bool IsLoaded() const
	{
		return m_loaded;
	}
	//クリップ名を取得
	const wchar_t* GetName() const
	{
		return m_clipName.c_str();
	}
	/*const GetFreezeBoneInfo*/
private:
	bool m_isLoop = false;										//!<ループフラグ。
	std::vector<Keyframe*> m_keyframes;							//全てのキーフレーム。
	std::vector<keyFramePtrList> m_keyFramePtrListArray;		//ボーンごとのキーフレームのリストを管理するための配列。
																//例えば、m_keyFramePtrListArray[0]は0番目のボーンのキーフレームのリスト、
	std::wstring m_clipName;															//m_keyFramePtrListArray[1]は1番目のボーンのキーフレームのリストといった感じ。
	std::unique_ptr<AnimationEvent[]>	m_animationEvent;		//アニメーションイベント。
	int									m_numAnimationEvent = 0;	//アニメーションイベントの数。
	using KeyframePtr = std::unique_ptr<Keyframe>;
	keyFramePtrList* m_topBoneKeyFramList = nullptr;
	SFreezeBoneInfo m_freezeBoneInfo;	//!<フリーズさせるボーン情報。
	bool m_loaded = false;	//アニメーションクリップがロードされている？
};
using CAnimationClipPtr = std::unique_ptr<AnimationClip>;

