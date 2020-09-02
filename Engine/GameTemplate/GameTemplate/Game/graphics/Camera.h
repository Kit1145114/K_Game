#pragma once
class Camera
{
public:
	/*!
	 * @brief	�J�����s��A�v���W�F�N�V�����s��̍X�V�B
	 *@details
	 * ���ݐݒ肳��Ă���A�����_�A���_�A��p�Ȃǂ̏������Ƃ�
	 * �J�����s��ƃv���W�F�N�V�����s����X�V���܂��B
	 * ���̊֐����Ă΂Ȃ��ƁA�J�����͓����Ȃ��̂Œ��ӂ��K�v�ł��B
	 */
	void Update();
	/*!
	 * @brief	�J�����s����擾�B
	 */
	CMatrix GetViewMatrix()
	{
		return m_viewMatrix;
	}
	/*!
	 * @brief	�v���W�F�N�V�����s����擾�B
	 */
	CMatrix GetProjectionMatrix() 
	{
		return m_projMatrix;
	}
	/*!
	 * @brief	���_���擾�B
	 */
	CVector3 GetPosition()
	{
		return m_position;
	}
	/*!
	 * @brief	�����_���擾�B
	 */
	CVector3 GetTarget()
	{
		return m_target;
	}
	/*!
	 * @brief	�J�����̏�������擾�B
	 */
	CVector3 GetUp()
	{
		return m_up;
	}
	/*!
* @brief	�J�����̑O���������擾�B
*/
	const CVector3& GetForward() const
	{
		return m_forward;
	}
	/*!
	* @brief	�J�����̉E�������擾�B
	*/
	const CVector3& GetRight() const
	{
		return m_right;
	}
	/*!
	 * @brief	���_��ݒ�B
	 */
	void SetPosition( CVector3 pos )
	{
		m_position = pos;
	}
	/*!
	 * @brief	�����_��ݒ�B
	 */
	void SetTarget( CVector3 target )
	{
		m_target = target;
	}
	/*!
	 * @brief	�J�����̏������ݒ�B
	 */
	void SetUp( CVector3 up )
	{
		m_up = up;
	}
	/*!
	 * @brief	�����ʂ�ݒ�B
	 */
	void SetFar( float f )
	{
		m_far = f;
	}
	/*!
	 * @brief	�ߕ��ʂ�ݒ�B
	 */
	void SetNear( float n )
	{
		m_near = n;
	}
	/*!
	 * @brief	��p��ݒ�B
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
	/// ��p���擾�B
	/// </summary>
	/// <returns>��p�B�P�ʃ��W�A��</returns>
	float GetViewAngle() const
	{
		return m_viewAngle;
	}
	/// <summary>
	/// �A�X�y�N�g����擾�B
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
	CMatrix	m_viewMatrix = CMatrix::Identity();		//�r���[�s��B
	CMatrix m_projMatrix = CMatrix::Identity();		//�v���W�F�N�V�����s��B
	CMatrix m_viewMatrixInv;						//�r���[�s��B
	CVector3 m_target = CVector3::Zero();			//�����_�B
	CVector3 m_position = CVector3::Zero();			//���_�B
	CVector3 m_up = CVector3::Up();					//������B
	CVector3 m_rotate = CVector3::Zero();
	CQuaternion m_Rot = CQuaternion::Identity();
	CVector3	m_forward = CVector3::Front();		//!<�J�����̑O���B
	CVector3	m_right = CVector3::Right();			//!<�J�����̉E�B
	float Angle = 0.0f;
	float m_viewAngle = CMath::DegToRad(60.0f);		//��p�B
	float m_far = 10000.0f;							//�������ʂ܂ł̋����B
	float m_near = 1.0f;							//�ߕ��ʂ܂ł̋����B
	CMatrix	m_viewMatrix2 = CMatrix::Identity();		//�r���[�s��B
	CMatrix m_projMatrix2 = CMatrix::Identity();		//�v���W�F�N�V�����s��B
};

extern Camera g_camera3D;		//!<3D�J�����B