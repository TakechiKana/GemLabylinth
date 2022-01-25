#include "stdafx.h"
#include "Prologue.h"
#include "Fade.h"
#include "Game.h"

namespace
{
	const float WIDTH = 1920.0f;
	const float HIGH = 1080.0f;
}

Prologue::Prologue()
{

}

Prologue::~Prologue()
{

}

bool Prologue::Start()
{
	m_spriteRender.Init("Assets/sprite/prologue/plo4_con.dds", WIDTH, HIGH);

	m_fade = FindGO<Fade>("fade");
	//�t�F�[�h�C�����J�n����B
	m_fade->StartFadeIn();
	return true;
}

void Prologue::Update()
{

	if (m_isWaitFadeout) {
		//�t�F�[�h������Ȃ�������B
		if (!m_fade->IsFade()) {
			NewGO<Game>(0, "game");
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

	//�摜�̍X�V
	m_spriteRender.Update();
}

void Prologue::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}