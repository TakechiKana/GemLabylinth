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
	//フェードインを開始する。
	m_fade->StartFadeIn();
	return true;
}

void Prologue::Update()
{

	if (m_isWaitFadeout) {
		//フェード中じゃなかったら。
		if (!m_fade->IsFade()) {
			NewGO<Game>(0, "game");
			//自身を削除する。
			DeleteGO(this);
		}
	}
	else {
		//Aボタンを押したら。
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_isWaitFadeout = true;
			//フェードアウトを開始する。
			m_fade->StartFadeOut();
			//効果音を再生する。
			/*SoundSource* se = NewGO<SoundSource>(0);
			se->Init(9);
			se->Play(false);
			se->SetVolume(0.6f);*/
		}
	}

	//画像の更新
	m_spriteRender.Update();
}

void Prologue::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}