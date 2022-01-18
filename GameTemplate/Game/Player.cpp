#include "stdafx.h"
#include "Player.h"
#include "Game.h"
//#include "ItemHeart.h"
#include "ItemDash.h"
#include "ItemMagic.h"
#include "MagicCollision.h"
#include "Enemy.h"
#include "Fade.h"

//CollisionObject���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"
//EffectEmitter���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "graphics/effect/EffectEmitter.h"


bool Player::Start()
{
	//�A�j���[�V�����̃��[�h
	animationClips[enAnimationClip_Idle].Load("Assets/animData/jackie/idle2.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Run].Load("Assets/animData/jackie/run.tka");
	animationClips[enAnimationClip_Run].SetLoopFlag(true);
	animationClips[enAnimationClip_FastRun].Load("Assets/animData/jackie/fastrun.tka");
	animationClips[enAnimationClip_FastRun].SetLoopFlag(true);
	animationClips[enAnimationClip_Magic].Load("Assets/animData/jackie/magic.tka");
	animationClips[enAnimationClip_Magic].SetLoopFlag(false);
	animationClips[enAnimationClip_Catch].Load("Assets/animData/jackie/catch.tka");
	animationClips[enAnimationClip_Catch].SetLoopFlag(true);
	//animationClips[enAnimationClip_Damage].Load("Assets/animData/jackie/receivedamage.tka");
	//animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	animationClips[enAnimationClip_Down].Load("Assets/animData/jackie/down.tka");
	animationClips[enAnimationClip_Down].SetLoopFlag(false);

	//Jackie���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/human/jackie.tkm", animationClips, enAnimationClip_Num, enModelUpAxisZ, true);
	m_modelRender.SetScale({2.0f,2.0f,2.0f});
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetShadowCasterFlag(true);
	m_modelRender.Update();
	//�L�����R��������������B
	m_characterController.Init(20.0f, 90.0f, m_position);

	//�A�j���[�V�����C�x���g
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//�{�[��
	m_PunchBoneId_R = m_modelRender.FindBoneID(L"mixamorig1:RightHand");

	m_gameOverRender.Init("Assets/sprite/kari/Gameover.dds", 1980.0f, 1080.0f);
	m_downTextRender.Init("Assets/sprite/life/Down_text.dds", 1980.0f, 1080.0f);
	m_downLife1Render.Init("Assets/sprite/life/life1.dds", 1980.0f, 1080.0f);
	m_downLife2Render.Init("Assets/sprite/life/life1.dds", 1980.0f, 1080.0f);
	m_downLife3Render.Init("Assets/sprite/life/life1.dds", 1980.0f, 1080.0f);

	//�e�N���X��FindGO
	m_dash = FindGO<ItemDash>("dash");
	m_magic = FindGO<ItemMagic>("magic");
	m_game = FindGO<Game>("game");
	m_enemys = FindGOs<Enemy>("enemy");

	m_fade = FindGO<Fade>("fade");

	return true;
}

Player::Player()
{
	
}

Player::~Player()
{

}

//�X�V�����B
void Player::Update()
{
	if (m_game->GetState() == Game::enGameState_Game) {
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

	}
	//�A�j���[�V�����̍Đ��B
	PlayAnimation();

	//wchar_t wcsbuf[256];
	//swprintf_s(wcsbuf, 256, L"%d",m_gemCount);
	////�擾��
	////�\������e�L�X�g��ݒ�B
	//fontRender.SetText(wcsbuf);
	////�t�H���g�̈ʒu��ݒ�B
	//fontRender.SetPosition(Vector3(-900.0f, 500.0f, 0.0f));
	////�t�H���g�̑傫����ݒ�B
	//fontRender.SetScale(2.0f);
	//swprintf_s(wcsbuf, 256, L"%.3f", m_game->GetTimer());
	////�擾��
	////�\������e�L�X�g��ݒ�B
	//fontRender1.SetText(wcsbuf);
	////�t�H���g�̈ʒu��ݒ�B
	//fontRender1.SetPosition(Vector3(-900.0f, 350.0f, 0.0f));
	////�t�H���g�̑傫����ݒ�B
	//fontRender1.SetScale(2.0f);

	//���f���̍X�V�B
	m_modelRender.Update();
}

//�����U������
void Player::MakeMagicCollision()
{
	MagicCollision* magicCollision = NewGO<MagicCollision>(0);
	Vector3 magicCollisionPos = m_position;
	//���W���v���C���[�̏����O�ɐݒ肷��B
	magicCollisionPos += m_forward * 50.0f;
	//���W�̈ʒu��������
	magicCollisionPos.y += 70.0f;
	magicCollision->SetSpeed(m_forward);
	//���W��ݒ�
	magicCollision->SetPosition(magicCollisionPos);
	magicCollision->SetRotation(m_rotation);

}

//���ݔ���
void Player::Catch()
{
	if (m_notCatchTimer > 0.0f) 
	{
		m_notCatchTimer -= GameTime().GetFrameDeltaTime();
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




	//if (m_playerState == enPlayerState_Run) {
	//	//���X�e�B�b�N�̓��͗ʂ�120.0f����Z�B
	//	right *= stickL.x * 350.0f;
	//	forward *= stickL.y * 350.0f;
	//}
	//if(m_playerState == enPlayerState_FastRun) {
	//	//���X�e�B�b�N�̓��͗ʂ�190.0f����Z�B
	//	right *= stickL.x * 450.0f;
	//	forward *= stickL.y * 450.0f;
	//}


	////�ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
	//m_moveSpeed += right + forward;


	/////////////////////////////////



	//�ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
	m_moveSpeed += right * stickL.x * 350.0f;
	m_moveSpeed += forward * stickL.y * 350.0f;

	//�_�b�V����ԂȂ瑬�x��1.6�{�ɂ���B
	if (m_playerState == enPlayerState_FastRun) {

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
		m_timer -= GameTime().GetFrameDeltaTime();
	}
	else 
	{
		m_timer = 0.0f;
	}
	ProcessState();
}

//�p���`�X�e�[�g
void Player::PunchState()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessState();
	}
}

//�����U���X�e�[�g
void Player::MagicState()
{
	if(m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessState();
	}
}

//�_�E���X�e�[�g
void Player::DownState()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		if (m_downCount == 0)
		{
			m_gameOver = true;
		}
		else {
			m_downScreen = true;
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				m_death = false;
				m_downScreen = false;
				m_playerState = enPlayerState_Idle;
			}
		}
	}
}

//�_���[�W�X�e�[�g
//void Player::DamageState()
//{
//	if (m_modelRender.IsPlayingAnimation() == false)
//	{
//		m_isUnderDamage = false;
//		//�X�e�[�g��J�ڂ���B
//		ProcessState();
//	}
//}

//�L���b�`�X�e�[�g
void Player::CatchState()
{
	ProcessState();
}

//�X�e�[�g�J��
void Player::ProcessState()
{
	if (m_playerState != enPlayerState_Catch) {
		////Y�{�^���������ꂽ��B
		//if (g_pad[0]->IsTrigger(enButtonY) && m_magicCount > 0)
		//{
		//	//�J�E���g-1
		//	m_magicCount -= 1;
		//	//�����U���X�e�[�g�Ɉڍs����B
		//	m_playerState = enPlayerState_Magic;

		//	return;
		//}
		
		//B�{�^���������ꂽ��B
		if (g_pad[0]->IsTrigger(enButtonB) && m_fastRun == false && m_dashCount > 0)
		{
			//�J�E���g-1
			m_dashCount -= 1;
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

	//x��z�̈ړ����x������������(�X�e�B�b�N�̓��͂�����������)�B
	else
	{
		//�X�e�[�g��ҋ@�ɂ���B
		m_playerState = enPlayerState_Idle;
	}
	//�_�E��������B
	if (m_death == true) 
	{
		m_downCount -= 1;
		m_playerState = enPlayerState_Down;
	}
	
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
		//Punch�X�e�[�g�̎��B
	case enPlayerState_Punch:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		PunchState();
		break;
		//���@�U���X�e�[�g�̎��B
	case enPlayerState_Magic:
		//���@�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		MagicState();
		break;
		//��_���[�W���X�e�[�g�̎��B
	//case enPlayerState_ReceiveDamage:
	//	//��_���[�W���X�e�[�g�̃X�e�[�g�J�ڏ����B
	//	DamageState();
	//	break;
		//�_�E���X�e�[�g�̎��B
	case enPlayerState_Down:
		//�_�E���X�e�[�g�̃X�e�[�g�J�ڏ����B
		DownState();
		break;
	case enPlayerState_Catch:
		CatchState();
		break;
		//�N���A�X�e�[�g�̎��B
	//case enPlayerState_Clear:
	//	//�N���A�X�e�[�g�̃X�e�[�g�J�ڏ����B
	//	ClearState();
	//	break;
	}
}

//�A�j���[�V�����̍Đ��B
void Player::PlayAnimation()
{
	//switch���B
	switch (m_playerState) {
	//�v���C���[�X�e�[�g��0(Idle)��������B
	case enPlayerState_Idle:
		//Idle
		m_modelRender.PlayAnimation(enAnimationClip_Idle,0.3f);
		break;
	//�v���C���[�X�e�[�g��1(Run)��������B
	case enPlayerState_Run:
		//Run
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.3f);
		break;
	//�v���C���[�X�e�[�g��2(FastRun)��������B
	case enPlayerState_FastRun:
		//FastRun
		m_modelRender.PlayAnimation(enAnimationClip_FastRun, 0.3f);
		break;
	//�v���C���[�X�e�[�g��4(Magic)��������B
	case enPlayerState_Magic:
		//Magic
		m_modelRender.PlayAnimation(enAnimationClip_Magic, 0.3f);
		break;
	//�v���C���[�X�e�[�g��5(Heal)��������B
	case enPlayerState_Catch:
		//Catch
		m_modelRender.PlayAnimation(enAnimationClip_Catch, 0.3f);
		break;
	//case enPlayerState_ReceiveDamage:
	//	//Damage
	//	m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.3f);
	//	break;
	case enPlayerState_Down:
		//Down
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.3f);
		break;
	default:
		break;
	}
}

//�A�j���[�V�����C�x���g�֐�
void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�L�[�̖��O���umagic_attack�v�̎��B
	if (wcscmp(eventName, L"magic_attack") == 0) {
		//magic���쐬����B
	MakeMagicCollision();
	}
}

//�`�揈���B
void Player::Render(RenderContext& rc)
{
	//���j�e�B������`�悷��B
	m_modelRender.Draw(rc);
	fontRender.Draw(rc);
	fontRender1.Draw(rc);
	fontRender2.Draw(rc);
	fontRender3.Draw(rc);
	if (m_downScreen == true) {
		m_downTextRender.Draw(rc);
	}
	if (m_gameOver == true)
	{
		m_gameOverRender.Draw(rc);
	}
}

//memo
/**/