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
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/jackie/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/jackie/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Punch].Load("Assets/animData/jackie/punch.tka");
	m_animationClips[enAnimationClip_Punch].SetLoopFlag(false);
	//m_animationClips[enAnimationClip_MagicAttack].Load("Assets/animData/enemy/magicattack.tka");
	//m_animationClips[enAnimationClip_MagicAttack].SetLoopFlag(false);
	//m_animationClips[enAnimationClip_Damage].Load("Assets/animData/enemy/receivedamage.tka");
	//m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	//m_animationClips[enAnimationClip_Down].Load("Assets/animData/enemy/down.tka");
	//m_animationClips[enAnimationClip_Down].SetLoopFlag(false);

	//���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/human/jackie.tkm", m_animationClips, enAnimationClip_Num);
	//�i�r���b�V��
	m_nvmMesh.Init("Assets/modelData/stage/stage_mesh.tkn");

	//���W��ݒ肷��B
	Vector3 position = m_position;
	position.y += 2.5f;

	m_modelRender.SetScale({2.0f,2.0f,2.0f});

	m_modelRender.SetPosition(position);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);
	//�傫����ݒ肷��B
	//m_modelRender.SetScale(m_scale);

	//�L�����N�^�[�R���g���[���[���������B
	m_charaCon.Init(
		25.0f,			//���a�B
		100.0f,			//�����B
		m_position		//���W�B
	);


	//����ǂݍ��ށB
	//g_soundEngine->ResistWaveFileBank(0, "Assets/sound/magic.wav");
	//g_soundEngine->ResistWaveFileBank(3, "Assets/sound/slash.wav");
	//g_soundEngine->ResistWaveFileBank(4, "Assets/sound/hit.wav");

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
	//�U�������B
	Attack();
	//�A�j���[�V�����̍Đ��B
	PlayAnimation();
	//�X�e�[�g�̑J�ڏ����B
	ManageState();

	//���f���̍X�V�B
	m_modelRender.Update();

	//�^�[�Q�b�g�|�W�V�����i�v���C���[�j�̍X�V
	m_targetPosition = m_player->GetPosition() + padding ;
}

void Enemy::Chase()
{

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
		4.5f,
		isEnd
	);



	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
	m_charaCon.SetPosition(m_position);
	Vector3 pos = Vector3::Zero;
	m_charaCon.Execute(pos, 1.0f);
}

void Enemy::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
		return;
	}
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
	//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
	m_rotation.SetRotationY(-angle);

	//�v���C���[�̑O�x�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);

}

void Enemy::Collision()
{
	//��_���[�W�A���邢�̓_�E���X�e�[�g�̎��́B
	//�����蔻�菈���͂��Ȃ��B
	if (m_enemyState == enEnemyState_ReceiveDamage ||
		m_enemyState == enEnemyState_Down)
	{
		return;
	}

	//�v���C���[�̍U���p�̃R���W�������擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_punch");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon))
		{
			//HP��1���炷�B
			m_hp -= 1;

			//����HP��0����Ȃ�B
			if (m_hp > 0)
			{
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_enemyState = enEnemyState_ReceiveDamage;
			}
			//HP��0�Ȃ�B
			else if (m_hp == 0)
			{
				//�_�E���X�e�[�g�ɑJ�ڂ���B
				m_enemyState = enEnemyState_Down;
			}

			//���ʉ����Đ�����B
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(4);
			se->Play(false);
			se->SetVolume(0.4f);
			return;
		}
	}

	//�v���C���[�̃t�@�C���[�{�[���p�̃R���W�������擾����B
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_fireball");
	//for���Ŕz����񂷁B
	for (auto collision : collisions2)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon))
		{
			//HP��1���炷�B
			m_hp -= 1;
			//HP��0�ɂȂ�����B
			if (m_hp == 0)
			{
				//�_�E���X�e�[�g�ɑJ�ڂ���B
				m_enemyState = enEnemyState_Down;
			}
			else {
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_enemyState = enEnemyState_ReceiveDamage;
			}
			//���ʉ����Đ�����B
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(4);
			se->Play(false);
			se->SetVolume(0.6f);
			return;
		}
	}
}

void Enemy::Attack()
{
	//�U���X�e�[�g�łȂ��Ȃ珈�������Ȃ��B
	if (m_enemyState != enEnemyState_Punch)
	{
		return;
	}

	//�U�����ł���΁B
	if (m_isUnderAttack == true)
	{
		//�U���p�̃R���W�������쐬����B
		MakePunchCollision();
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

void Enemy::MakePunchCollision()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);

	Vector3 collisionPosition = m_position;
	//���W���v���C���[�̏����O�ɐݒ肷��B
	collisionPosition += m_forward * 50.0f;
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateSphere(collisionPosition,               //���W�B
		Quaternion::Identity,                                      //��]�B
		20.0f												       //�傫���B
	);
	collisionObject->SetName("enemy_punch");

	//�uPunch�v�{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelRender.GetBone(m_PunchBoneId)->GetWorldMatrix();
	//�uPunch�v�{�[���̃��[���h�s����R���W�����ɓK�p����B
	collisionObject->SetWorldMatrix(matrix);
}

void Enemy::ProcessState()
{
	//�v���C���[����������B
	if (SearchPlayer() == true)
	{
		//�v���C���[�Ɍ������đ���
		m_enemyState = enEnemyState_Chase;
		//�ʏ�U���ł��鋗���Ȃ�
		if (IsCanPunch() == true)
		{	
			m_enemyState = enEnemyState_Punch;
		}
	}
	//�v���C���[���������Ȃ���΁B
	else
	{
		//�ҋ@�X�e�[�g�ɑJ�ڂ���B
		m_enemyState = enEnemyState_Idle;
		return;
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
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessState();
	}
}

void Enemy::DamageState()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessState();
	}
}

void Enemy::DownState()
{
	//�_�E���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		Game* game = FindGO<Game>("game");
		//�|���ꂽ�G�l�~�[�̐���+1����B
		//���g���폜����B
		DeleteGO(this);
	}
}

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
		//��_���[�W�X�e�[�g�̎��B
	case enEnemyState_ReceiveDamage:
		//��_���[�W�X�e�[�g�̃X�e�[�g�J�ڏ����B
		DamageState();
		break;
		//�_�E���X�e�[�g�̎��B
	case enEnemyState_Down:
		//�_�E���X�e�[�g�̃X�e�[�g�J�ڏ����B
		DownState();
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
	//	//���@�U���X�e�[�g�̎��B
	//case enEnemyState_MagicAttack:
	//	m_modelRender.SetAnimationSpeed(1.2f);
	//	//���@�U���A�j���[�V�������Đ��B
	//	m_modelRender.PlayAnimation(enAnimationClip_MagicAttack, 0.3f);
	//	break;
	//	//��_���[�W�X�e�[�g�̎��B
	//case enEnemyState_ReceiveDamage:
	//	m_modelRender.SetAnimationSpeed(1.3f);
	//	//��_���[�W�A�j���[�V�������Đ��B
	//	m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.3f);
	//	break;
	//	//�_�E���X�e�[�g�̎��B
	//case enEnemyState_Down:
	//	//�_�E���A�j���[�V�������Đ��B
	//	m_modelRender.PlayAnimation(enAnimationClip_Down, 0.3f);
	//	break;
	default:
		break;
	}
}

const bool Enemy::IsCanPunch() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//�G�l�~�[�ƃv���C���[�̋������߂�������B
	if (diff.Length() <= 90.0f)
	{
		//�U���ł���I
		return true;
	}
	//�U���ł��Ȃ��B
	return false;
}

void Enemy::Render(RenderContext& rc)
{
	//���f����`�悷��B
	m_modelRender.Draw(rc);
}