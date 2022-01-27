#include "stdafx.h"
#include "Player.h"
#include "Game.h"
#include "Clock.h"
#include "ItemDash.h"
#include "Enemy.h"
#include "Fade.h"
#include "Score.h"

//CollisionObject���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"
//EffectEmitter���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "graphics/effect/EffectEmitter.h"

namespace
{
	const float WIDTH = 1920.f;
	const float HIGH = 1080.f;
	const float WINGS_SPRITE = 170.0f;
	const float HEART_SPRITE = 130.0f;
}

bool Player::Start()
{
	//�A�j���[�V�����̃��[�h
	animationClips[enAnimationClip_Idle].Load("Assets/animData/jackie/idle2.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Run].Load("Assets/animData/jackie/run.tka");
	animationClips[enAnimationClip_Run].SetLoopFlag(true);
	animationClips[enAnimationClip_FastRun].Load("Assets/animData/jackie/fastrun.tka");
	animationClips[enAnimationClip_FastRun].SetLoopFlag(true);
	animationClips[enAnimationClip_Catch].Load("Assets/animData/jackie/catch.tka");
	animationClips[enAnimationClip_Catch].SetLoopFlag(true);
	animationClips[enAnimationClip_Down].Load("Assets/animData/jackie/down.tka");
	animationClips[enAnimationClip_Down].SetLoopFlag(false);
	animationClips[enAnimationClip_StandUp].Load("Assets/animData/jackie/standup.tka");
	animationClips[enAnimationClip_StandUp].SetLoopFlag(false);

	//Jackie���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/human/jackie.tkm", animationClips, enAnimationClip_Num, enModelUpAxisZ, true);
	m_modelRender.SetScale({2.0f,2.0f,2.0f});
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetShadowCasterFlag(true);
	m_modelRender.Update();
	//�L�����R��������������B
	m_characterController.Init(20.0f, 90.0f, m_position);

	m_life1Render.Init("Assets/sprite/life/life1.dds", HEART_SPRITE, HEART_SPRITE);
	m_life1Render.SetPosition(Vector3(0.0f, 10.0f, 0.0f));
	m_life2Render.Init("Assets/sprite/life/life2.dds", HEART_SPRITE, HEART_SPRITE);
	m_life1Render.SetPosition(Vector3(0.0f, 10.0f, 0.0f));
	m_life3Render.Init("Assets/sprite/life/life3.dds", HEART_SPRITE, HEART_SPRITE);
	m_life1Render.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_wingsRender.Init("Assets/sprite/UI/wings.dds", WINGS_SPRITE, WINGS_SPRITE);
	m_wingsRender.SetPosition(Vector3(-830.0f, 350.0f, 0.0f));

	//�e�N���X��FindGO
	m_dash = FindGO<ItemDash>("dash");
	m_game = FindGO<Game>("game");
	m_enemys = FindGOs<Enemy>("enemy");

	m_fade = FindGO<Fade>("fade");
	m_isGamenow = true;
	return true;
}

Player::Player()
{
	
}

Player::~Player()
{
	m_score = FindGO<Score>("score");
	m_score->SetUseItem(m_useItem);
	m_score->SetLeftGem(m_gemCount);
}

//�X�V�����B
void Player::Update()
{
	//�ړ������B
	Move();
	//��]�����B
	Rotation();
	//�X�e�[�g�̑J�ڏ����B
	ManageState();
	//���ݔ���
	Catch();
	//�N���A����
	Clear();
	//�A�j���[�V�����̍Đ��B
	PlayAnimation();
	//�����\��
	Font();
	//���f���̍X�V�B
	m_modelRender.Update();
	m_wingsRender.Update();
	m_life1Render.Update();
	m_life2Render.Update();
	m_life3Render.Update();
}

void Player::Clear() {
	if (m_gemCount == 0 && m_isGamenow == true)
	{
		NewGO<Clock>(0, "clock");
	}
}

//�����\��
void Player::Font()
{
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"x %d", m_dashCount);
	//�\������e�L�X�g��ݒ�B
	m_leftdash_Render.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�B
	m_leftdash_Render.SetPosition(Vector3(-760.0f, 400.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_leftdash_Render.SetScale(2.0f);
	swprintf_s(wcsbuf, 256, L"A�ŗ����オ��");
	//�\������e�L�X�g��ݒ�B
	m_downCommentRender.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�B
	m_downCommentRender.SetPosition(Vector3(0.0f, -200.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_downCommentRender.SetScale(1.5f);
	swprintf_s(wcsbuf, 256, L"���̂����� �̂���%d", m_gemCount);
	//�\������e�L�X�g��ݒ�B
	m_gemRender.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�B
	m_gemRender.SetPosition(Vector3(100.0f, 500.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_gemRender.SetScale(1.5f);
	
}

//���ݔ���
void Player::Catch()
{
	if (m_notCatchTimer > 0.0f) 
	{
		m_notCatchTimer -= g_gameTime->GetFrameDeltaTime();
		
	}
	else
	{
		m_notCatchTimer = 0.0f;
	}
}

//�ړ�����
void Player::Move()
{
	if (IsEnableMove() == false)
	{
		return;
	}
	//xz�̈ړ����x��0.0f�ɂ���B
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ��擾�B
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	//y�����ɂ͈ړ������Ȃ��B
	forward.y = 0.0f;
	right.y = 0.0f;

	//�ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
	m_moveSpeed += right * stickL.x * 350.0f;
	m_moveSpeed += forward * stickL.y * 350.0f;

	//�_�b�V����ԂȂ瑬�x��1.6�{�ɂ���B
	if (m_playerState == enPlayerState_FastRun) 
	{
		m_moveSpeed.x *= 1.3f;
		m_moveSpeed.z *= 1.3f;
	}


	////////////////////////////////


	//�n�ʂɕt���Ă�����B
	if (m_characterController.IsOnGround())
	{
		//�d�͂𖳂����B
		m_moveSpeed.y = 0.0f;
	}
	//�n�ʂɕt���Ă��Ȃ�������B
	else
	{
		//�d�͂𔭐�������B
		m_moveSpeed.y -= 2.5f;
	}

	//�L�����N�^�[�R���g���[���[���g���č��W���ړ�������B
	m_position = m_characterController.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	//�G�`������ɍ��W��������B
	m_modelRender.SetPosition(m_position);
}

//��]
void Player::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
		return;
	}
	//atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐��B
	//m_moveSpeed.x / m_moveSpeed.z�̌��ʂ�tan�ƂɂȂ�B
	//atan2���g�p���āA�p�x�����߂Ă���B
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
	//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
	m_rotation.SetRotationY(-angle);

	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);

	//�v���C���[�̐��ʃx�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

//�A�C�h���X�e�[�g
void Player::IdleState()
{
	ProcessState();
}

//�ړ��X�e�[�g
void Player::RunState()
{
	ProcessState();
}

//���x�A�b�v�X�e�[�g
void Player::FastRunState()
{
	if (m_timer > 0.0f) {
		m_timer -= g_gameTime->GetFrameDeltaTime();
	}
	else 
	{
		m_timer = 0.0f;
	}
	ProcessState();
}

//�L���b�`�X�e�[�g
void Player::CatchState()
{
	//�_�E��������B
	if (m_death == true)
	{
		m_downCount -= 1;
		m_playerState = enPlayerState_Down;
	}

}

//�_�E���X�e�[�g
void Player::DownState()
{
	//�A�j���[�V�������Đ����Ȃ�B
	if (m_modelRender.IsPlayingAnimation() == true)
	{
		//�������Ȃ��B
		//���߂̃��^�[���B
		//�������^�[���B
		return;
	}

	//�_�E���J�E���g��0�Ȃ�B
	if (m_downCount ==0)
	{
		//�Q�[���I�[�o�[�B
		m_game->SetGameOverFlag();
		m_isGamenow = false;
		return;
	}

	m_downScreen = true;
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_death = false;
		m_playerState = enPlayerState_StandUp;
	}
}

void Player::StandUpState()
{
	if (m_modelRender.IsPlayingAnimation() == true)
	{
		//�������Ȃ��B
		return;
	}
	m_playerState = enPlayerState_Idle;
}

//�X�e�[�g�J��
void Player::ProcessState()
{
	if (m_playerState != enPlayerState_Catch) {
		//B�{�^���������ꂽ��B
		if (g_pad[0]->IsTrigger(enButtonB) && m_fastRun == false && m_dashCount > 0)
		{
			//�J�E���g-1
			m_dashCount -= 1;
			m_useItem += 1;
			//�_�b�V���^�C����4�b�ɐݒ�B
			m_timer = 4.0f;
			m_fastRun = true;
			return;
		}
	}
	for (int i = 0; i < 2; i++) {
		if (m_enemys[i]->GetCatchState() == true)
		{
			m_playerState = enPlayerState_Catch;
			return;
		}
	}

	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		if (m_timer > 0.0f) {
			m_playerState = enPlayerState_FastRun;
		}
		else {
			//�X�e�[�g�𑖂�ɂ���B
			m_playerState = enPlayerState_Run;
			m_fastRun = false;
		}
		return;
	}
	//�X�e�[�g��ҋ@�ɂ���B
	m_playerState = enPlayerState_Idle;
	
	
}

//�X�e�[�g�Ǘ��B
void Player::ManageState()
{
	switch (m_playerState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enPlayerState_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ����B
		IdleState();
		break;
		//Run�X�e�[�g�̎��B
	case enPlayerState_Run:
		//Run�X�e�[�g�̃X�e�[�g�J�ڏ����B
		RunState();
		break;
		//FastRun�X�e�[�g�̎��B
	case enPlayerState_FastRun:
		//FastRun�X�e�[�g�̃X�e�[�g�J�ڏ����B
		FastRunState();
		break;
		//�_�E���X�e�[�g�̎��B
	case enPlayerState_Down:
		//�_�E���X�e�[�g�̃X�e�[�g�J�ڏ����B
		DownState();
		break;
	case enPlayerState_Catch:
		CatchState();
		break;
		//�N���A�X�e�[�g�̎��B
	case enPlayerState_StandUp:
		//�N���A�X�e�[�g�̃X�e�[�g�J�ڏ����B
		StandUpState();
		break;
	default:
		break;
	}
}

//�A�j���[�V�����̍Đ��B
void Player::PlayAnimation()
{
	//switch���B
	switch (m_playerState) {
	//�v���C���[�X�e�[�g��Idle��������B
	case enPlayerState_Idle:
		//Idle
		m_modelRender.PlayAnimation(enAnimationClip_Idle,0.3f);
		break;
	//�v���C���[�X�e�[�g��Run��������B
	case enPlayerState_Run:
		//Run
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.3f);
		break;
	//�v���C���[�X�e�[�g��FastRun��������B
	case enPlayerState_FastRun:
		//FastRun
		m_modelRender.PlayAnimation(enAnimationClip_FastRun, 0.3f);
		break;
	//�v���C���[�X�e�[�g��Catch��������B
	case enPlayerState_Catch:
		//Catch
		m_modelRender.PlayAnimation(enAnimationClip_Catch, 0.3f);
		break;
	//�v���C���[�X�e�[�g��Down��������B
	case enPlayerState_Down:
		//Down
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.3f);
		break;
	case enPlayerState_StandUp:
		//Down
		m_modelRender.PlayAnimation(enAnimationClip_StandUp, 0.3f);
		break;
	default:
		break;
	}
}

//�`�揈���B
void Player::Render(RenderContext& rc)
{
	//���f����`�悷��B
	m_modelRender.Draw(rc);
	m_gemRender.Draw(rc);
	m_wingsRender.Draw(rc);
	m_life1Render.Draw(rc);
	m_life2Render.Draw(rc);
	m_life3Render.Draw(rc);
	//if (m_downCount >= 3)
	//{
	//	m_life3Render.Draw(rc);
	//}
	//if (m_downCount >= 2)
	//{
	//	m_life2Render.Draw(rc);
	//}
	//
	m_leftdash_Render.Draw(rc);
	if (m_death == true) {
		m_downCommentRender.Draw(rc);
	}
}
