#include "stdafx.h"
#include "Title.h"

#include "Prologue.h"
#include "Game.h"
#include "Fade.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

namespace
{
	const float WIDHT = 1920.0f;
}

Title::~Title()
{
	//BGMを削除する。
	//DeleteGO(m_bgm);
}

bool Title::Start()
{
	//画像を読み込む。
	m_spriteRender.Init("Assets/sprite/title/Title_jackie1.dds", WIDHT, 1080);
	m_play.Init("Assets/sprite/title/title_play.dds",450,128);
	m_play.SetPosition(Vector3(0.0f, -250.0f, 0.0f));

	//音を読み込む。
	/*g_soundEngine->ResistWaveFileBank(8, "Assets/sound/titlebgm.wav");
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/button.wav");*/

	//BGM。
	/*m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(8);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.3f);*/

	m_fade = FindGO<Fade>("fade");
	//フェードインを開始する。
	m_fade->StartFadeIn();
	return true;
}

void Title::Update()
{
	if (m_isWaitFadeout) {
		//フェード中じゃなかったら。
		if (!m_fade->IsFade()) {
			NewGO<Prologue>(0, "prologue");
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

	//α値を変化させる。
	if (m_isWaitFadeout)
	{
		m_alpha += g_gameTime->GetFrameDeltaTime() * 20.5f;
	}
	else
	{
		m_alpha += g_gameTime->GetFrameDeltaTime() * 1.2f;
	}

	m_play.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));

	//画像の更新。
	m_spriteRender.Update();
	m_play.Update();
}

void Title::Render(RenderContext& rc)
{
	//画像の描画。
	m_spriteRender.Draw(rc);
	m_play.Draw(rc);
}