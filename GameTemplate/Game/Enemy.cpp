#include "stdafx.h"
#include "Enemy.h"

#include "Game.h"
#include "Player.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

#include <time.h>
#include <stdlib.h>

//CollisionObject���g�p���������߁A�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"

Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

bool Enemy::Start()
{
	//�A�j���[�V������ǂݍ��ށB
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/michelle/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/michelle/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Punch].Load("Assets/animData/michelle/punch.tka");
	m_animationClips[enAnimationClip_Punch].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Phose].Load("Assets/animData/michelle/sad.tka");
	m_animationClips[enAnimationClip_Phose].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/michelle/receivedamage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	//m_animationClips[enAnimationClip_Down].Load("Assets/animData/michelle/down.tka");
	//m_animationClips[enAnimationClip_Down].SetLoopFlag(false);

	//���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/human/Michelle.tkm", m_animationClips, enAnimationClip_Num);
	//�i�r���b�V��
	m_nvmMesh.Init("Assets/modelData/stage/stage_mesh2.tkn");

	//���W��ݒ肷��B
	Vector3 position = m_position;
	position.y += 2.5f;

	m_modelRender.SetScale({2.7f,2.7f,2.7f});

	m_modelRender.SetPosition(position);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);
	//�傫����ݒ肷��B
	//m_modelRender.SetScale(m_scale);

	//�L�����N�^�[�R���g���[���[���������B
	m_charaCon.Init(
		40.0f,			//���a�B
		135.0f,			//�����B
		m_position		//���W�B
	);


	//����ǂݍ��ށB
	//g_soundEngine->ResistWaveFileBank(0, "Assets/sound/magic.wav");
	//g_soundEngine->ResistWaveFileBank(3, "Assets/sound/slash.wav");
	//g_soundEngine->ResistWaveFileBank(4, "Assets/sound/hit.wav");

	//�A�j���[�V�����C�x���g�̏���
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//�ǂ̃{�[����
	m_PunchBoneId = m_modelRender.FindBoneID(L"mixamorig:RightHand");

	//player�N���X�̌���
	m_player = FindGO<Player>("player");
	//�������������B
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	m_targetPosition = m_player->GetPosition();
	return true;
}

void Enemy::Update()
{
	//�ǐՏ���
	Chase();
	//��]�����B
	Rotation();
	//�����蔻��B
	Collision();


	//�A�j���[�V�����̍Đ��B
	PlayAnimation();
	//�X�e�[�g�̑J�ڏ����B
	ManageState();
	//�U���ł��邩
	IsCanPunch();

	//���f���̍X�V�B
	m_modelRender.Update();

	//�^�[�Q�b�g�|�W�V�����i�v���C���[�j�̍X�V
	m_targetPosition = m_player->GetPosition() + padding ;
}

void Enemy::Chase()
{
	if (m_enemyState != enEnemyState_Chase)
	{
		return;
	}
	//�|�[�Y����
	Phose();
	bool isEnd;
	if (SearchPlayer()==true)   {
		// �p�X����
		m_pathFiding.Execute(
			m_path,							// �\�z���ꂽ�p�X�̊i�[��
			m_nvmMesh,						// �i�r���b�V��
			m_position,						// �J�n���W
			m_targetPosition,				// �ړ��ڕW���W
			PhysicsWorld::GetInstance(),	// �����G���W��	
			10.0f,							// AI�G�[�W�F���g�̔��a
			200.0f							// AI�G�[�W�F���g�̍����B
		);
	}

	// �p�X����ړ�����B
	m_position = m_path.Move(
		m_position,
		4.0f,
		isEnd,
		m_velocity
	);

	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
	m_charaCon.SetPosition(m_position);
	Vector3 pos = Vector3::Zero;
	m_charaCon.Execute(pos, 1.0f);
}

void Enemy::Rotation()
{
	//if (fabsf(m_velocity.x) < 0.001f
	//	&& fabsf(m_velocity.z) < 0.001f) {
	//	//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
	//	//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
	//	return;
	//}

	Quaternion rotation;
	rotation.SetRotationYFromDirectionXZ(m_velocity);
	m_modelRender.SetRotation(rotation);

}

void Enemy::Phose()
{
	if (m_catchTimer > 0.0f)
	{
		m_catchTimer -= GameTime().GetFrameDeltaTime();
	}
	else
	{
		return;
	}
}

void Enemy::Collision()
{
	//��_���[�W�A���邢�̓_�E���X�e�[�g�̎��́B
	//�����蔻�菈���͂��Ȃ��B
	if (m_enemyState == enEnemyState_ReceiveDamage)
	{
		return;
	}

	//�v���C���[�̃}�W�b�N�p�̃R���W�������擾����B
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_magic");
	//for���Ŕz����񂷁B
	for (auto collision : collisions2)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon))
		{
			m_enemyState = enEnemyState_ReceiveDamage;
			////���ʉ����Đ�����B
			////����
			return;
		}
	}
}

const bool Enemy::SearchPlayer() const
{
	//�G�l�~�[����v���C���[�Ɍ������x�N�g�������߂�B
	Vector3 diff = m_player->GetPosition() - m_position;

	//�v���C���[�ɂ�����x�߂�������B
	if (diff.LengthSq() <= 1000.0f*1000.0f)
	{
		return true;
	}
	//�v���C���[���������Ȃ������B
	return false;
}

void Enemy::ProcessState()
{
	if (m_catchTimer <= 0.0f) {
		//�v���C���[����������B
		if (SearchPlayer() == true)
		{
			//�ʏ�U���ł��鋗���Ȃ�
			if (m_catch == true)
			{
				m_enemyState = enEnemyState_Punch;

			}
			else
			{
				//�v���C���[�Ɍ������đ���
				m_enemyState = enEnemyState_Chase;
			}
		}
		//�v���C���[���������Ȃ���΁B
		else
		{
			//�ҋ@�X�e�[�g�ɑJ�ڂ���B
			m_enemyState = enEnemyState_Idle;
		}
		if (m_isUnderDamage == true) {
			m_enemyState = enEnemyState_ReceiveDamage;
		}
	}
}

void Enemy::IdleState()
{
	//���̃X�e�[�g�֑J�ڂ���B
	ProcessState();
}

void Enemy::WalkState()
{
	//���̃X�e�[�g�ɑJ�ڂ���B
	ProcessState();
}

void Enemy::RunState()
{
	//���̃X�e�[�g�ɑJ�ڂ���B
	ProcessState();
}

void Enemy::ChaseState()
{
	//���̃X�e�[�g�ɑJ�ڂ���B
	ProcessState();
}

void Enemy::PunchState()
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_catchTimer = 20.0f;
		m_catch = false;
		m_enemyState = enEnemyState_Phose;
	}
}
//�����̃|�[�Y�֐�
void Enemy::PhoseState()
{
	if (m_catchTimer < 0.0f)
	{
		m_catchTimer = 0.0f;
		m_enemyState = enEnemyState_Idle;
	}
}
//�_���[�W�֐�
void Enemy::DamageState()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		
		ProcessState();
		m_isUnderDamage = false;
	}
}

//�e�X�e�[�g�̊֐��Ăяo��
void Enemy::ManageState()
{
	switch (m_enemyState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enEnemyState_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ����B
		IdleState();
		break;
		//�ǐՃX�e�[�g�̎��B
	case enEnemyState_Chase:
		//�ǐՃX�e�[�g�̃X�e�[�g�J�ڏ����B
		ChaseState();
		break;
		//�U���X�e�[�g�̎��B
	case enEnemyState_Punch:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		PunchState();
		break;
	case enEnemyState_Phose:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		PhoseState();
		break;
		//��_���[�W�X�e�[�g�̎��B
	case enEnemyState_ReceiveDamage:
		//��_���[�W�X�e�[�g�̃X�e�[�g�J�ڏ����B
		DamageState();
		break;
	default:
		break;
	}
}

void Enemy::PlayAnimation()
{
	switch (m_enemyState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enEnemyState_Idle:
		//�ҋ@�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.3f);
		break;
		//�ǐՃX�e�[�g�̎��B
	case enEnemyState_Chase:
		//����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.3f);
		break;
		//�U���X�e�[�g�̎��B
	case enEnemyState_Punch:
		//�U���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Punch, 0.3f);
		break;
	case enEnemyState_Phose:
		//�U���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Phose, 0.3f);
		break;
		//��_���[�W�X�e�[�g�̎��B
	case enEnemyState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.3f);
		//��_���[�W�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.3f);
		break;
		//�_�E���X�e�[�g�̎��B
	default:
		break;
	}
}

void Enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�L�[�̖��O���uattack_end�v�̎��B
	if (wcscmp(eventName, L"attack_end") == 0) {
		//�U�����I���B
		m_player->RaceiveDamage();
	}
}

void Enemy::IsCanPunch()
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//�G�l�~�[�ƃv���C���[�̋������߂�������B
	if (diff.Length() <= 80.0f && m_catchTimer <= 0.0f)
	{
		//�U���ł���I
		m_catch = true;
	}
	else
	{
		//�U���ł��Ȃ��B
		m_catch = false;
	}
}

void Enemy::Render(RenderContext& rc)
{
	//���f����`�悷��B
	m_modelRender.Draw(rc);
}