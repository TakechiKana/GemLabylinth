#include "stdafx.h"
#include "Score.h"

#include "Title.h"
#include "Game.h"
#include "Fade.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

namespace
{
	const float WIDHT = 1920.0f;
}

Score::~Score()
{
	//BGM���폜����B
	//DeleteGO(m_bgm);
}

bool Score::Start()
{
	//�摜��ǂݍ��ށB
	m_spriteRender.Init("Assets/sprite/score/score.dds", WIDHT, 1080);

	//����ǂݍ��ށB
	/*g_soundEngine->ResistWaveFileBank(8, "Assets/sound/titlebgm.wav");
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/button.wav");*/

	//BGM�B
	/*m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(8);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.3f);*/

	m_fade = FindGO<Fade>("fade");
	//�t�F�[�h�C�����J�n����B
	m_fade->StartFadeIn();
	return true;
}

void Score::Update()
{
	if (m_isWaitFadeout) {
		//�t�F�[�h������Ȃ�������B
		if (!m_fade->IsFade()) {
			NewGO<Title>(0, "title");
			//���g���폜����B
			DeleteGO(this);
		}
	}
	else {
		//A�{�^������������B
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_isWaitFadeout = true;
			//�t�F�[�h�A�E�g���J�n����B
			m_fade->StartFadeOut();
			//���ʉ����Đ�����B
			/*SoundSource* se = NewGO<SoundSource>(0);
			se->Init(9);
			se->Play(false);
			se->SetVolume(0.6f);*/
		}
	}
	ManageState();
	//�摜�̍X�V�B
	m_spriteRender.Update();
}

void Score::ClearFont()
{
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"���ݎ����@�@�@AM %02d:%02d", m_hour, m_minutes);
	//�擾��
	//�\������e�L�X�g��ݒ�B
	m_time.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�B
	m_time.SetPosition(Vector3(0.0f, 300, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_time.SetScale(2.0f);

	swprintf_s(wcsbuf, 256, L"�g�����A�C�e�����@�@�@�@%d", m_item);
	//�擾��
	//�\������e�L�X�g��ݒ�B
	m_useitem.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�B
	m_useitem.SetPosition(Vector3(0.0f, 100, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_useitem.SetScale(2.0f);
	swprintf_s(wcsbuf, 256, L"Game Clear");
	//�擾��
	//�\������e�L�X�g��ݒ�B
	m_gameclear.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�B
	m_gameclear.SetPosition(Vector3(0.0f, -300, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_gameclear.SetScale(2.5f);
	if (m_hour >= 4)
	{
		swprintf_s(wcsbuf, 256, L"�����Ɠ������ԂɋN�����I�����͂�H�ׂ悤�B");
		//�擾��
		//�\������e�L�X�g��ݒ�B
		m_comment1.SetText(wcsbuf);
		//�t�H���g�̈ʒu��ݒ�B
		m_comment1.SetPosition(Vector3(0.0f, -100, 0.0f));
		//�t�H���g�̑傫����ݒ�B
		m_comment1.SetScale(1.5f);
	}
	else
	{
		swprintf_s(wcsbuf, 256, L"�����͂�����葁���N��������������B�\�K���悤���ȁH");
		//�擾��
		//�\������e�L�X�g��ݒ�B
		m_comment2.SetText(wcsbuf);
		//�t�H���g�̈ʒu��ݒ�B
		m_comment2.SetPosition(Vector3(0.0f, -100, 0.0f));
		//�t�H���g�̑傫����ݒ�B
		m_comment2.SetScale(1.5f);
	}
}

void Score::GameOverFont()
{
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"���ݎ����@�@�@AM 05:00");
	//�擾��
	//�\������e�L�X�g��ݒ�B
	m_time.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�B
	m_time.SetPosition(Vector3(0.0f, 300, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_time.SetScale(2.0f);

	swprintf_s(wcsbuf, 256, L"�g�����A�C�e�����@�@�@�@%d", m_item);
	//�擾��
	//�\������e�L�X�g��ݒ�B
	m_useitem.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�B
	m_useitem.SetPosition(Vector3(0.0f, 100, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_useitem.SetScale(2.0f);
	swprintf_s(wcsbuf, 256, L"�ǂ��܂ł����Ă����{�B�Q�Ă��o�߂Ă����{�c�B");
	//�擾��
	//�\������e�L�X�g��ݒ�B
	m_gameover.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�B
	m_gameover.SetPosition(Vector3(0.0f, -300, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_gameover.SetScale(1.5f);
}

void Score::ManageState()
{
	switch (m_scoreState) 
	{
	case enScoreState_Clear:
		ClearFont();
		break;
	case enScoreState_GameOver:
		GameOverFont();
		break;
	default:
		break;
	}
}

void Score::Render(RenderContext& rc)
{
	//�摜�̕`��B
	m_spriteRender.Draw(rc);
}