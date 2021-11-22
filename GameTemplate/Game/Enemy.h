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
		enEnemyState_Magic,					//�����U���B
		enEnemyState_ReceiveDamage,			//��_���[�W�B
		enEnemyState_Down,					//�_�E��
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
	//HP�𑼂�����炤
	void SetHP(const int hp)
	{
		m_hp = hp;
	}
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

private:
	//�ړ�����
	void Move();
	//�ǐՏ���
	void Chase();
	//��]����
	void Rotation();
	//�U������
	void Attack();
	//�����蔻��
	void Collision();
	//�v���C���[�T��
	const bool SearchPlayer() const;
	//�p���`�̓����蔻��R���W��������
	void MakePunchCollision();
	//�X�e�[�W2�ȍ~��������Ȃ�g�p�\��
	//void MakeMagic();
	//�A�j���[�V�����Đ�
	void PlayAnimation();
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
	/// ���@�U���X�e�[�g�̑J�ڏ����B
	//void MagicState();
	/// ��_���[�W�X�e�[�g�̑J�ڏ����B
	void DamageState();
	/// �_�E���X�e�[�g�̑J�ڏ����B
	void DownState();
	//�U���ł��鋗���̔���
	const bool IsCanPunch() const;

	enum EnAnimationClip {						//�A�j���[�V�����B
		enAnimationClip_Idle,					//�ҋ@�A�j���[�V�����B
		enAnimationClip_Walk,					//�����A�j���[�V�����B
		enAnimationClip_Run,					//����A�j���[�V�����B
		enAnimationClip_Punch,					//�U���A�j���[�V�����B
		//enAnimationClip_Magic,				//���@�U���A�j���[�V�����B
		enAnimationClip_Damage,					//��_���[�W�A�j���[�V�����B
		enAnimationClip_Down,					//�_�E���A�j���[�V�����B
		enAnimationClip_Num,					//�A�j���[�V�����̐��B
	};
	AnimationClip				m_animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
	ModelRender					m_modelRender;								//���f�������_�[�B
	Vector3						m_position;									//���W�B
	Vector3						m_targetPosition;							//�^�[�Q�b�g�̍��W
	Vector3						m_moveSpeed;								//�ړ����x�B
	Vector3						m_forward = Vector3::AxisZ;					//�G�l�~�[�̐��ʃx�N�g���B
	Quaternion					m_rotation;									//��]�B
	Vector3						m_scale = Vector3::One;						//�傫���B
	CharacterController			m_charaCon;									//�L�����R���B
	EnEnemyState				m_enemyState = enEnemyState_Idle;			//�G�l�~�[�X�e�[�g�B
	TknFile m_tknFile;
	nsAI::NaviMesh m_nvmMesh;
	nsAI::Path m_path;
	nsAI::PathFinding m_pathFiding;
	bool						m_isUnderAttack = false;					//�U�������H
	int							m_hp = 0;									//HP�B
	Player*						m_player = nullptr;							//�v���C���[�B
	float						m_chaseTimer = 0.0f;						//�ǐՃ^�C�}�[�B
	float						m_idleTimer = 0.0f;
	int							m_PunchBoneId = -1;							//�ځ[��ID

};

