#pragma once
class Player;

class Door : public IGameObject
{
public:
	enum EnDoorState {
		enDoorState_CloseIdle,			//�̑ҋ@��ԁB
		enDoorState_OpenIdle,			//�J�̑ҋ@��ԁB
		enDoorState_Open,				//�I�[�v�����B
		enDoorState_Close				//�N���[�Y���B
	};
public:
	Door();
	~Door();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// ���W�̐ݒ�B
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// �h�A�̔ԍ���ݒ肷��B
	/// </summary>
	/// <param name="doorNumber">�h�A�̔ԍ��B</param>
	void SetDoorNumber(const int doorNumber)
	{
		m_doorNumber = doorNumber;
	}
	/// <summary>
	/// �h�A�̔ԍ����擾����B
	/// </summary>
	/// <returns>�h�A�̔ԍ��B</returns>
	const int GetDoorNumber() const
	{
		return m_doorNumber;
	}
	/// <summary>
	/// �I�[�v����ʒm����B
	/// </summary>
	void NotifyOpen();
	/// <summary>
	/// �N���[�Y��ʒm����B
	/// </summary>
	void NotifyClose();

//�����o�֐�
private:
	/// <summary>
	/// �A�j���[�V�����Đ��B
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// �I�u�W�F�N�g�̓����蔻����J���B
	/// </summary>
	void ReleasePhysicsObject();
	/// <summary>
	/// �I�u�W�F�N�g�̓����蔻����쐬�B
	/// </summary>
	void CreatePhysicsObject();
	/// <summary>
	/// �X�e�[�g�Ǘ��B
	/// </summary>
	void ManageState();
	/// <summary>
	/// �I�[�v���X�e�[�g�J�ڏ����B
	/// </summary>
	void ProcessOpenStateTransition();
	/// <summary>
	/// �N���[�Y�X�e�[�g�J�ڏ����B
	/// </summary>
	void ProcessCloseStateTransition();

//�����o�ϐ�
private:
	enum EnAnimationClip //�A�j���[�V�����B
	{
		enAnimationClip_CloseIdle,	//�ҋ@�A�j���[�V�����B(���Ă���Ƃ�)
		enAnimationClip_OpenIdle,	//�ҋ@�A�j���[�V�����B(�J���Ă���Ƃ�)
		enAnimationClip_Open,		//�I�[�v���A�j���[�V�����B
		enAnimationClip_Close,		//�N���[�Y�A�j���[�V�����B
		enAnimationClip_Num,		//�A�j���[�V�������B
	};

	Player* m_player;
	ModelRender m_modelRender;
	Vector3 m_position;
	Vector3 m_scale;
	AnimationClip				m_animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
	PhysicsStaticObject			m_physicsStaticObject;						//�t�B�W�N�X�X�^�e�B�b�N�I�u�W�F�N�g�B	
	EnDoorState					m_doorState = enDoorState_CloseIdle;		//�h�A�X�e�[�g�B
	int							m_doorNumber = 0;							//�h�A�̔ԍ��B

};
