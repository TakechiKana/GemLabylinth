#pragma once

//�N���X�錾
class Collision;
class ItemDash;
class ItemHeart;
class ItemMagic;
class ItemPunchUp;

 
//�v���C���[�N���X�B
class Player : public IGameObject
{
public:
	//�X�e�[�g
	enum EnPlayerState {
		enPlayerState_Idle,					//�ҋ@�B
		enPlayerState_Run,					//����B
		enPlayerState_FastRun,				//��������B
		enPlayerState_Punch,				//�U��
		enPlayerState_PunchUp,				//�U���̓A�b�v
		enPlayerState_Magic,				//�����U���B
		enPlayerState_Healing,				//�񕜁B
		enPlayerState_ReceiveDamage,		//�_���\�W�󂯂��B
		enPlayerState_Down,					//HP��0�B
		//enPlayerState_Clear					//�N���A�[�B
	};

	bool Start();
	Player();
	~Player();

	float GetHealth()
	{
		return m_health;
	}

	int GetState()const
	{
		return m_playerState;
	}

	//�_�b�V���^�C�}�[����n���֐�
	float GetDashTimer() const
	{
		return m_timer;
	}
	//�^�C�}�[����n���֐�
	float GetHealthTimer() const
	{
		return m_timer1;
	}

	//�|�W�V������n���֐�
	const Vector3 GetPosition() const
	{
		return m_position;
	}
	//�|�W�V�����̑��
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	//��]�ݒ�
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	//�ړ����x��n���֐�
	Vector3& SetMoveSpeed(const Vector3& movespeed)
	{
		m_moveSpeed = movespeed;
	}

	//�X�V�����B
	void Update();
	//�`�揈���B
	void Render(RenderContext& rc);
	//�p���`
	void Punch();
	//�p���`�R���W����
	void MakePunchCollision();
	//�����U���R���W����
	void MakeMagicCollision();
	//�R���W�����̔���
	void Collision();
	//�̗́A�_���[�W�A��
	void Health();
	//�A�j���[�V�����C�x���g�p�֐�
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//�ړ������B
	void Move();
	//��]�����B
	void Rotation();
	//�X�e�[�g�Ǘ��B
	void ManageState();
	//�X�e�[�g�J��
	void ProcessState();
	//�A�C�h���X�e�[�g
	void IdleState();
	//�����X�e�[�g
	void RunState();
	//�t�@�X�g�����X�e�[�g
	void FastRunState();
	//�p���`�X�e�[�g
	void PunchState();
	//�����U���X�e�[�g
	void MagicState();
	//�񕜃X�e�[�g
	void HealState();
	//�_���[�W�X�e�[�g
	void DamageState();
	//�_�E���X�e�[�g�̎�
	void DownState();
	////�N���A�X�e�[�g
	//void ClearState();

	//�A�j���[�V�����̍Đ��B
	void PlayAnimation();
	
	bool IsEnableMove() const
	{
		return m_playerState != enPlayerState_Healing &&
			m_playerState != enPlayerState_Magic &&
			m_playerState != enPlayerState_ReceiveDamage &&
			m_playerState != enPlayerState_Punch &&
			m_playerState != enPlayerState_PunchUp

			/*&&
			m_playerState != enPlayerState_Down &&
			m_playerState != enPlayerState_Clear*/;
	}

private:
	//�����o�ϐ��B
	ModelRender m_modelRender;	//���f�������_�\�B
	Vector3 m_position;			//���W�B
	enum EnAnimationClip {		//�A�j���[�V�����B
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_FastRun,
		enAnimationClip_Punch,
		enAnimationClip_Magic,
		enAnimationClip_Heal,
		enAnimationClip_Num,
	};
	AnimationClip animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
	CharacterController m_characterController;				//�L�����N�^�[�R���g���[���[�B
	Vector3 m_moveSpeed;									//�ړ����x�B
	Vector3 m_forward;										//�O�������x�N�g��
	Quaternion m_rotation;									//�N�H�[�^�j�I���B
	int m_playerState = enPlayerState_Idle;					//�v���C���[�X�e�[�g
	int m_health = 4;										//HP
	float m_timer = 0.0f;									//�_�b�V���^�C��
	float m_timer1 = 0.0f;									//�N�[���^�C��
	bool m_downState = false;								//�_�E���̔���
	bool m_isUnderAttack = false;							//�U�����̔���
	bool m_fastRun = false;									//���x�A�b�v���̔���
	int m_PunchBoneId = -1;									//�ځ[��ID

	//�e�N���X�̕ϐ�
	class ItemDash;
	class ItemHeart;
	class ItemMagic;
	class ItemPunchUp;

	FontRender fontRender;
};