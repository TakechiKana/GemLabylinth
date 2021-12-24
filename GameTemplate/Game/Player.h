#pragma once


//�N���X�錾
class Collision;
class ItemDash;
class ItemMagic;
class Game;
class Enemy;
class Fade;

 
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
		enPlayerState_Magic,				//�����U���B
		enPlayerState_Healing,				//�񕜁B
		//enPlayerState_ReceiveDamage,		//�_���\�W�󂯂��B
		enPlayerState_Down,					//HP��0�B
		enPlayerState_Catch,				//���܂����B
		enPlayerState_Clear					//�N���A�[�B
	};

	bool Start();
	Player();
	~Player();

	//�_���[�W����
	void RaceiveDamage()
	{
		m_death = true;
	}

	//�_�b�V���A�C�e�����E������
	void GetDashCount()
	{
		m_dashCount += 1;
	}
	//�}�W�b�N�A�C�e�����E������
	void GetMagicCount()
	{
		m_magicCount += 1;
	}
	//�W�F�����E������
	void GetGemCount()
	{
		m_gemCount += 1;
	}

	//�|�W�V������n���֐�
	const Vector3 GetPosition() const
	{
		return m_position;
	}

	//�|�W�V�������N���X�O�Őݒ�
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	//�ăX�^�[�g�n�_�̐ݒ�
	void SetRestartPosition(const Vector3& position)
	{
		m_restartPos = position;
	}

	//��]���N���X�O�Őݒ�
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	//player�̑O����
	const Vector3 GetForward() const
	{
		return m_forward;
	}
	////�N���A
	bool Clear() {
		if (m_gemCount == 178 && m_clear == true)
		{
			return true;
		}
	}
	
	bool IsEnableMove() const
	{
		return m_playerState != enPlayerState_Healing &&
			m_playerState != enPlayerState_Magic &&
			m_playerState != enPlayerState_Punch &&
			m_playerState != enPlayerState_Down &&
			m_playerState != enPlayerState_Catch &&
			m_playerState != enPlayerState_Clear;
	}

private:
	//���ݔ���
	void Catch();
	//�A�j���[�V�����C�x���g�p�֐�
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
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
	//�_���[�W�X�e�[�g
	//void DamageState();
	//�_�E���X�e�[�g�̎�
	void DownState();
	//�L���b�`�X�e�[�g
	void CatchState();
	//�X�V�����B
	void Update();
	//�`�揈���B
	void Render(RenderContext& rc);
	//�����U���R���W����
	void MakeMagicCollision();
	//�ړ������B
	void Move();
	//��]�����B
	void Rotation();
	//�A�j���[�V�����̍Đ��B
	void PlayAnimation();
	//�_�E�����̎c�@�\��
	//void LifeRender(RenderContext& rc);


	//�����o�ϐ��B
	enum EnAnimationClip //�A�j���[�V�����B
	{		
		enAnimationClip_Idle,		//�ҋ@�A�j���[�V�����B
		enAnimationClip_Run,		//����A�j���[�V�����B
		enAnimationClip_FastRun,	//�_�b�V���A�C�e�����̑���A�j���[�V�����B
		enAnimationClip_Magic,		//�}�W�b�N�A�j���[�V�����B
		enAnimationClip_Catch,		//���܂ꂽ�A�j���[�V�����B
		//enAnimationClip_Damage,		//�_���[�W�A�j���[�V�����B
		enAnimationClip_Down,		//�_�E���A�j���[�V�����B
		enAnimationClip_Num,		//�A�j���[�V�������B
	};
	
	Vector3 m_moveSpeed;									//�ړ����x�B
	Vector3 m_forward;										//�O�������x�N�g��
	Vector3 m_position;										//���W�B
	Vector3 m_restartPos;									//�ăX�^�[�g�n�_
	Quaternion m_rotation;									//�N�H�[�^�j�I���B
	EnPlayerState m_playerState = enPlayerState_Idle;		//�v���C���[�X�e�[�g
	ModelRender m_modelRender;								//���f�������_�\�B
	AnimationClip animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
	CharacterController m_characterController;				//�L�����N�^�[�R���g���[���[�B

	int m_health = 3;										//HP

	float m_timer = 0.0f;									//�_�b�V���^�C��
	float m_timer1 = 0.0f;									//�}�W�b�N�폜�܂�
	float m_notCatchTimer = 0.0f;							//���G����

	bool m_fastRun = false;									//���x�A�b�v���̔���

	int m_dashCount = 0;									//�_�b�V���A�C�e���J�E���g
	int m_magicCount = 0;									//�}�W�b�N�A�C�e���J�E���g
	int m_gemCount = 0;										//�W�F���J�E���g

	bool m_death = false;									//�_�E���������H
	bool m_downScreen = false;							//�_���[�W�������Ă��邩�H
	bool m_clear = false;

	int	m_PunchBoneId_R= -1;									//BoneID

	//�e�N���X�̕ϐ�
	ItemDash* m_dash;
	Game* m_game;
	ItemMagic* m_magic;

	Fade* m_fade;

	std::vector<Enemy*>  m_enemys;

	SpriteRender m_downTextRender;		//�_�E�����e�L�X�g
	SpriteRender m_downLife1Render;		//�_�E�����c�@�P
	SpriteRender m_downLife2Render;		//�_�E�����c�@�Q
	SpriteRender m_downLife3Render;		//�_�E�����c�@�R


	FontRender fontRender;
	FontRender fontRender1; 
	FontRender fontRender2; 
	FontRender fontRender3;
};
