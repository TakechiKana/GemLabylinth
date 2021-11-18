#include "stdafx.h"
#include "Player.h"

//CollisionObject���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"
//EffectEmitter���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "graphics/effect/EffectEmitter.h"


bool Player::Start()
{
	//�A�j���[�V�����̃��[�h
	animationClips[enAnimationClip_Idle].Load("Assets/animData/jackie/idle.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Run].Load("Assets/animData/jackie/run.tka");
	animationClips[enAnimationClip_Run].SetLoopFlag(true);
	animationClips[enAnimationClip_FastRun].Load("Assets/animData/jackie/fastrun.tka");
	animationClips[enAnimationClip_FastRun].SetLoopFlag(true);
	animationClips[enAnimationClip_Punch].Load("Assets/animData/jackie/punch.tka");
	animationClips[enAnimationClip_Punch].SetLoopFlag(false);
	animationClips[enAnimationClip_Magic].Load("Assets/animData/jackie/magic.tka");
	animationClips[enAnimationClip_Magic].SetLoopFlag(false);
	animationClips[enAnimationClip_Heal].Load("Assets/animData/jackie/heal.tka");
	animationClips[enAnimationClip_Heal].SetLoopFlag(false);
	//Jackie���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/human/jackie.tkm", animationClips, enAnimationClip_Num, enModelUpAxisZ);
	m_modelRender.SetScale({2.0f,2.0f,2.0f});


	//�L�����R��������������B
	m_characterController.Init(20.0f, 90.0f, m_position);

	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	m_PunchBoneId = m_modelRender.FindBoneID(L"mixamorig1:RightHand");
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
	//�ړ������B
	Move();
	//��]�����B
	Rotation();
	//�����蔻��B
	Collision();
	//�U�������B
	Punch();
	//�X�e�[�g�̑J�ڏ����B
	ManageState();
	//�A�j���[�V�����̍Đ��B
	PlayAnimation();

	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"%f", m_timer);
	//�擾��
	//�\������e�L�X�g��ݒ�B
	fontRender.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�B
	fontRender.SetPosition(Vector3(-900.0f, 500.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	fontRender.SetScale(2.0f);

	/////////////////////////////
	if (g_pad[0]->IsPress(enButtonLB2))
	{
		//�񕜃X�e�[�g�Ɉڍs����B
		m_playerState = enPlayerState_Healing;

		return;
	}
	////////////////////////////

	//���f���̍X�V�B
	m_modelRender.Update();
}

//�U������
void Player::MakePunchCollision()
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
	collisionObject->SetName("player_punch");

	//�uPunch�v�{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelRender.GetBone(m_PunchBoneId)->GetWorldMatrix();
	//�uPunch�v�{�[���̃��[���h�s����R���W�����ɓK�p����B
	collisionObject->SetWorldMatrix(matrix);
}

//�����U������
void Player::MakeMagicCollision()
{
	return;
}

//�R���W�����̔���
void Player::Collision()
{
	//��_���[�W�A�_�E�����A�N���A���̓_���[�W��������Ȃ��B
	if (m_playerState == enPlayerState_ReceiveDamage /*||
		m_playerState == enPlayerState_Down ||
		m_playerState == enPlayerState_Clear*/)
	{
		return;
	}
}

//�U���̔���
void Player::Punch()
{
	//�U�����łȂ��Ȃ�A���������Ȃ��B
	if (m_playerState != enPlayerState_Punch&&
		m_playerState != enPlayerState_PunchUp)
	{
		return;
	}

	//�U�����蒆�ł���΁B
	if (m_isUnderAttack == true)
	{
		//�U���p�̃R���W�������쐬����B
		MakePunchCollision();
	}
}

void Player::Health()
{
	if (m_playerState == enPlayerState_Healing) {
		if (g_pad[0]->IsTrigger(enButtonX))
		{
			if (m_health < 4) {
				m_health += 1;
			}
			else
			{
				return;
			}
			return;
		}
	}
	if (m_playerState == enPlayerState_Down) {

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

		m_moveSpeed.x *= 1.6f;
		m_moveSpeed.z *= 1.6f;
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
	m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

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
	else {
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

//�񕜃X�e�[�g
void Player::HealState()
{
	Health();
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessState();
	}
}

void Player::DownState()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessState();
	}
}

//�X�e�[�g�J��
void Player::ProcessState()
{
	
	//X�{�^���������ꂽ��B
	if (g_pad[0]->IsTrigger(enButtonX))
	{
		//�񕜃X�e�[�g�Ɉڍs����B
		m_playerState = enPlayerState_Healing;
		
		return;
	}
	//X�{�^���������ꂽ��B
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		//�񕜃X�e�[�g�Ɉڍs����B
		m_playerState = enPlayerState_Magic;

		return;
	}
	//A�{�^���������ꂽ��B
	else if (g_pad[0]->IsTrigger(enButtonRB2))
	{
		//�U���̓A�b�v�X�e�[�g�Ɉڍs����B
		m_playerState = enPlayerState_Punch;
		
		return;
	}
	//�U���͂��A�b�v����
	if (g_pad[0]->IsPress(enButtonA))
	{
		if (g_pad[0]->IsTrigger(enButtonRB2)) {
			//�U���̓A�b�v�X�e�[�g�Ɉڍs����B
			m_playerState = enPlayerState_PunchUp;
			//���o�[���������߂̃R���W�������쐬����B
			return;
		}
	}

	//B�{�^���������ꂽ��B
	if (g_pad[0]->IsTrigger(enButtonB)&&m_fastRun == false)
	{
		//�_�b�V���^�C����4�b�ɐݒ�B
		m_timer = 4.0f;
		m_fastRun = true;
		return;
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
		return;
	}
	if (m_health == 0) {
		m_playerState = enPlayerState_Down;
	}
	//�_�b�V���p�̃^�C�����X�V�B
	/*if (m_timer > 0.0f) {
		m_timer -= GameTime().GetFrameDeltaTime();
	}
	else {
		m_timer = 0.0f;
	}*/
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
		//�񕜃X�e�[�g�̎��B
	case enPlayerState_Healing:
		//�񕜎��X�e�[�g�̃X�e�[�g�J�ڏ����B
		HealState();
		break;
		//��_���[�W���X�e�[�g�̎��B
	case enPlayerState_ReceiveDamage:
		//��_���[�W���X�e�[�g�̃X�e�[�g�J�ڏ����B
		//DamageState();
		break;

		//�_�E���X�e�[�g�̎��B
	/*case enPlayerState_Down:
		//�_�E���X�e�[�g�̃X�e�[�g�J�ڏ����B
		DownState();
		break;
		//�N���A�X�e�[�g�̎��B
	case enPlayerState_Clear:
		//�N���A�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ClearState();
		break;
	}*/
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
		//�v���C���[�X�e�[�g��3(Punch)��������B
	case enPlayerState_Punch:
		//Punch
		m_modelRender.PlayAnimation(enAnimationClip_Punch, 0.3f);
		break;
		//�v���C���[�X�e�[�g��3(Punch)��������B
	case enPlayerState_PunchUp:
		//Punch
		m_modelRender.PlayAnimation(enAnimationClip_Punch, 0.3f);
		break;
		//�v���C���[�X�e�[�g��4(Magic)��������B
	case enPlayerState_Magic:
		//Magic
		m_modelRender.PlayAnimation(enAnimationClip_Magic, 0.3f);
		break;
		//�v���C���[�X�e�[�g��5(Heal)��������B
	case enPlayerState_Healing:
		//Healing
		m_modelRender.PlayAnimation(enAnimationClip_Heal, 0.3f);
		break;
	}
}

//�A�j���[�V�����C�x���g�֐�
void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�L�[�̖��O���uattack_start�v�̎��B
	if (wcscmp(eventName, L"attack_start") == 0) {
		//�U�����ɂ���B
		m_isUnderAttack = true;
		//�G�t�F�N�g�𔭐�������B
		//MakeSlashingEffect();
	}
	//�L�[�̖��O���uattack_end�v�̎��B
	else if (wcscmp(eventName, L"attack_end") == 0) {
		//�U�����I���B
		m_isUnderAttack = false;
	}
	//�L�[�̖��O���umagic_attack�v�̎��B
	else if (wcscmp(eventName, L"magic_attack") == 0) {
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
}

//memo
/**/