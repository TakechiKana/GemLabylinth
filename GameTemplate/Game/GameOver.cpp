#include "stdafx.h"
#include "GameOver.h"
#include "Title.h"
#include "Fade.h"
#include "Score.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

namespace
{
	const float WIDHT = 1920.0f;
}

GameOver::~GameOver()
{
	//BGM���폜����B
	//DeleteGO(m_bgm);
}

bool GameOver::Start()
{
	//�摜��ǂݍ��ށB
	m_spriteRender.Init("Assets/sprite/gameover/gameover.dds", WIDHT, 1080);

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

void GameOver::Update()
{
	if (m_isWaitFadeout) {
		//�t�F�[�h������Ȃ�������B
		if (!m_fade->IsFade()) {
			NewGO<Score>(0, "score");
			m_score->SetGameOver();
			m_score->SetUseItem(m_useitem);
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

	//�摜�̍X�V�B
	m_spriteRender.Update();
}

void GameOver::Render(RenderContext& rc)
{
	//�摜�̕`��B
	m_spriteRender.Draw(rc);
}