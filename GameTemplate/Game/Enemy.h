#pragma once

#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"

class Player;

class Enemy : public IGameObject
{
public:
	//�G�l�~�[�X�e�[�g
	enum EnEnemyState {
		enEnemyState_Idle,					//�ҋ@�B
		enEnemyState_Chase,					//�ǐՁB
		enEnemyState_Punch,					//�U���B
		enEnemyState_Phose,					//�����̈ꎞ��~
		enEnemyState_Magic,					//�����U���B
		enEnemyState_ReceiveDamage,			//��_���[�W�B
	};

public:
	Enemy();
	~Enemy();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	//////////���N���X�֓n�����߂̊֐�//////////
	//���W�𑼂�����炤
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	//���W��n��
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	//��]��ݒ�
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	//�L���b�`�̔����n��
	const bool GetCatchState() const
	{
		return m_catch;
	}
	const float GetCatchTimer() const
	{
		return m_catchTimer;
	}
private:
	//�ړ�����
	void Move();
	//�ǐՏ���
	void Chase();
	//��]����
	void Rotation();
	//�����蔻��
	void Collision();
	//�|�[�Y���̏���
	void Phose();
	//�v���C���[�T��
	const bool SearchPlayer() const;
	//�A�j���[�V�����Đ�
	void PlayAnimation();
	//�A�j���[�V�����C�x���g�p�֐�
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//�X�e�[�g�J�ڏ���
	void ManageState();
	/// ���ʂ̃X�e�[�g�J�ڏ����B
	void ProcessState();
	/// �ҋ@�X�e�[�g�̑J�ڏ����B
	void IdleState();
	/// �����X�e�[�g�̑J�ڏ����B
	void WalkState();
	/// ����X�e�[�g�̑J�ڏ����B
	void RunState();
	/// �ǐՃX�e�[�g�̔w�J�ڏ����B
	void ChaseState();
	/// �U���X�e�[�g�̑J�ڏ����B
	void PunchState();
	/// �����ꎞ��~�X�e�[�g�̑J�ڏ����B
	void PhoseState();
	/// ��_���[�W�X�e�[�g�̑J�ڏ����B
	void DamageState();
	//�U���ł��鋗���̔���
	void IsCanPunch();

	enum EnAnimationClip {						//�A�j���[�V�����B
		enAnimationClip_Idle,					//�ҋ@�A�j���[�V�����B
		enAnimationClip_Walk,					//�����A�j���[�V�����B
		enAnimationClip_Run,					//����A�j���[�V�����B
		enAnimationClip_Punch,					//�U���A�j���[�V�����B
		enAnimationClip_Phose,					//�U����ꎞ��~�A�j���[�V�����B
		enAnimationClip_Damage,					//��_���[�W�A�j���[�V�����B
		enAnimationClip_Num,					//�A�j���[�V�����̐��B
	};
	AnimationClip				m_animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
	ModelRender					m_modelRender;								//���f�������_�[�B
	Vector3						m_position;									//���W�B
	Vector3						m_targetPosition;							//�^�[�Q�b�g�̍��W
	Vector3						m_moveSpeed;								//�ړ����x�B
	Vector3						m_forward = Vector3::AxisZ;					//�G�l�~�[�̐��ʃx�N�g���B
	Vector3						padding = { 20.0f,20.0f,20.0f };			//�^�[�Q�b�g���v���C���[��菭�������B
	Quaternion					m_rotation;									//��]�B
	Vector3						m_scale = Vector3::One;						//�傫���B
	CharacterController			m_charaCon;									//�L�����R���B
	EnEnemyState				m_enemyState = enEnemyState_Idle;			//�G�l�~�[�X�e�[�g�B

	//�i�r���b�V��
	TknFile m_tknFile;
	nsAI::NaviMesh m_nvmMesh;
	nsAI::Path m_path;
	nsAI::PathFinding m_pathFiding;
	////

	bool						m_isUnderDamage = false;					//�_���[�W���󂯂���
	bool						m_catch = false;							//�v���C���[��߂܂����H
	float						m_catchTimer = 0.0f;
	Player*						m_player = nullptr;							//�v���C���[�B
	int							m_PunchBoneId = -1;							//�ځ[��ID
	Vector3						m_velocity = Vector3::Zero;					//�ړ�����
};

