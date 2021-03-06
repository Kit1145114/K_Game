#pragma once
class Camera
{
public:
	/*!
	 * @brief	カメラ行列、プロジェクション行列の更新。
	 *@details
	 * 現在設定されている、注視点、視点、画角などの情報をもとに
	 * カメラ行列とプロジェクション行列を更新します。
	 * この関数を呼ばないと、カメラは動かないので注意が必要です。
	 */
	void Update();
	/*!
	 * @brief	カメラ行列を取得。
	 */
	CMatrix GetViewMatrix()
	{
		return m_viewMatrix;
	}
	/*!
	 * @brief	プロジェクション行列を取得。
	 */
	CMatrix GetProjectionMatrix() 
	{
		return m_projMatrix;
	}
	/*!
	 * @brief	視点を取得。
	 */
	CVector3 GetPosition()
	{
		return m_position;
	}
	/*!
	 * @brief	注視点を取得。
	 */
	CVector3 GetTarget()
	{
		return m_target;
	}
	/*!
	 * @brief	カメラの上方向を取得。
	 */
	CVector3 GetUp()
	{
		return m_up;
	}
	/*!
* @brief	カメラの前方方向を取得。
*/
	const CVector3& GetForward() const
	{
		return m_forward;
	}
	/*!
	* @brief	カメラの右方向を取得。
	*/
	const CVector3& GetRight() const
	{
		return m_right;
	}
	/*!
	 * @brief	視点を設定。
	 */
	void SetPosition( CVector3 pos )
	{
		m_position = pos;
	}
	/*!
	 * @brief	注視点を設定。
	 */
	void SetTarget( CVector3 target )
	{
		m_target = target;
	}
	/*!
	 * @brief	カメラの上方向を設定。
	 */
	void SetUp( CVector3 up )
	{
		m_up = up;
	}
	/*!
	 * @brief	遠平面を設定。
	 */
	void SetFar( float f )
	{
		m_far = f;
	}
	/*!
	 * @brief	近平面を設定。
	 */
	void SetNear( float n )
	{
		m_near = n;
	}
	/*!
	 * @brief	画角を設定。
	 */
	void SetViewAngle( float angle )
	{
		m_viewAngle = angle;
	}

	void SetRotate(CVector3 rot)
	{
		m_rotate = rot;
	}

	void SetRotation(CQuaternion qrot)
	{
		m_Rot = qrot;
	}

	void SetAngle(float angle)
	{
		Angle = angle;
	}
	float GetNear()
	{
		return m_near;
	}
	float GetFar()
	{
		return m_far;
	}
	CVector3& GetRight()
	{
		return m_right;
	}
	/// <summary>
	/// 画角を取得。
	/// </summary>
	/// <returns>画角。単位ラジアン</returns>
	float GetViewAngle() const
	{
		return m_viewAngle;
	}
	/// <summary>
	/// アスペクト比を取得。
	/// </summary>
	float GetAspect() const
	{
		return 1280.0f / 720.0f;
	}
	CMatrix& GetViewMatrix2()
	{
		return m_viewMatrix2;
	}
	
private:
	CMatrix	m_viewMatrix = CMatrix::Identity();		//ビュー行列。
	CMatrix m_projMatrix = CMatrix::Identity();		//プロジェクション行列。
	CMatrix m_viewMatrixInv;						//ビュー行列。
	CVector3 m_target = CVector3::Zero();			//注視点。
	CVector3 m_position = CVector3::Zero();			//視点。
	CVector3 m_up = CVector3::Up();					//上方向。
	CVector3 m_rotate = CVector3::Zero();
	CQuaternion m_Rot = CQuaternion::Identity();
	CVector3	m_forward = CVector3::Front();		//!<カメラの前方。
	CVector3	m_right = CVector3::Right();			//!<カメラの右。
	float Angle = 0.0f;
	float m_viewAngle = CMath::DegToRad(60.0f);		//画角。
	float m_far = 10000.0f;							//遠い平面までの距離。
	float m_near = 1.0f;							//近平面までの距離。
	CMatrix	m_viewMatrix2 = CMatrix::Identity();		//ビュー行列。
	CMatrix m_projMatrix2 = CMatrix::Identity();		//プロジェクション行列。
};

extern Camera g_camera3D;		//!<3Dカメラ。