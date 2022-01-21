#include "stdafx.h"
#include "Door.h"
#include "Player.h"

Door::Door()
{
}

Door::~Door()
{
}

bool Door::Start()
{
	//���f���ǂݍ���
	m_modelRender.Init("Assets/modelData/stage/door.tkm", m_animationClips, enAnimationClip_Num);
	//�e�𗎂Ƃ��Ȃ�
	m_modelRender.SetShadowCasterFlag(false);
	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
	//�A�j���[�V������ǂݍ��ށB
	m_animationClips[enAnimationClip_CloseIdle].Load("Assets/animData/door/idle_close.tka");
	m_animationClips[enAnimationClip_CloseIdle].SetLoopFlag(false);
	m_animationClips[enAnimationClip_OpenIdle].Load("Assets/animData/door/idle_open.tka");
	m_animationClips[enAnimationClip_OpenIdle].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Open].Load("Assets/animData/door/open.tka");
	m_animationClips[enAnimationClip_Open].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Close].Load("Assets/animData/door/close.tka");
	m_animationClips[enAnimationClip_Close].SetLoopFlag(false);
	//���f���̍X�V�B
	m_modelRender.Update();
	//�����蔻����쐬����B
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	m_player = FindGO<Player>("player");
	return true;
}

void Door::Update()
{
	//�A�j���[�V�������Đ�����B
	PlayAnimation();
	//�X�e�[�g�Ǘ��B
	ManageState();

	//���f���̍X�V�B
	m_modelRender.Update();
}
void Door::NotifyOpen()
{
	//�I�[�v���X�e�[�g�ɑJ�ڂ���B
	m_doorState = enDoorState_Open;
	//�����Đ�����B
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(5);
	se->Play(false);
	se->SetVolume(0.7f);
}

void Door::NotifyClose()
{
	//�N���[�Y�X�e�[�g�ɑJ�ڂ���B
	m_doorState = enDoorState_Close;
	//�����Đ�����B
	/*SoundSource* se = NewGO<SoundSource>(0);
	se->Init(5);
	se->Play(false);
	se->SetVolume(0.6f);*/
}

void Door::PlayAnimation()
{
	switch (m_doorState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enDoorState_CloseIdle:
		//�ҋ@�A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimationClip_CloseIdle);
		break;
		//�ҋ@�X�e�[�g�̎��B
	case enDoorState_OpenIdle:
		//�ҋ@�A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimationClip_OpenIdle);
		break;
		//�I�[�v���X�e�[�g�̎��B
	case enDoorState_Open:
		//�I�[�v���A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimationClip_Open);
		m_modelRender.SetAnimationSpeed(0.6f);
		break;
		//�N���[�Y�X�e�[�g�̎��B
	case enDoorState_Close:
		//�N���[�Y�A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimationClip_Close);
		m_modelRender.SetAnimationSpeed(0.6f);
		break;
	default:
		break;
	}
}

void Door::ReleasePhysicsObject()
{
	//�����蔻����J������B
	m_physicsStaticObject.Release();
}

void Door::CreatePhysicsObject()
{
	//�����蔻����쐬����B
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
}

void Door::ProcessOpenStateTransition()
{
	//�I�[�v���A�j���[�V�����̍Đ����I��������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�����蔻����J������B
		ReleasePhysicsObject();
		//�I�[�v���I���X�e�[�g�ɑJ�ڂ���B
		m_doorState = enDoorState_OpenIdle;
	}
}

void Door::ProcessCloseStateTransition()
{
	//�N���[�Y�A�j���[�V�������I��������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�����蔻����쐬����B
		CreatePhysicsObject();
		//�N���[�Y�I���X�e�[�g�ɑJ�ڂ���B
		m_doorState = enDoorState_CloseIdle;
	}
}

void Door::ManageState()
{
	switch (m_doorState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case Door::enDoorState_CloseIdle:

		break;
		//�I�[�v���X�e�[�g�̎��B
	case Door::enDoorState_Open:
		//�I�[�v���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessOpenStateTransition();
		break;
		//�I�[�v���I���X�e�[�g�̎��B
	case Door::enDoorState_OpenIdle:

		break;
		//�N���[�Y�X�e�[�g�̎��B
	case Door::enDoorState_Close:
		//�N���[�Y�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessCloseStateTransition();
		break;

	default:
		break;
	}
}

void Door::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}