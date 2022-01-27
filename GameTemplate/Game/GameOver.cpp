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
	//BGMを削除する。
	//DeleteGO(m_bgm);
}

bool GameOver::Start()
{
	//画像を読み込む。
	m_spriteRender.Init("Assets/sprite/gameover/gameover.dds", WIDHT, 1080);

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

void GameOver::Update()
{
	if (m_isWaitFadeout) {
		//フェード中じゃなかったら。
		if (!m_fade->IsFade()) {
			NewGO<Score>(0, "score");
			m_score->SetGameOver();
			m_score->SetUseItem(m_useitem);
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

	//画像の更新。
	m_spriteRender.Update();
}

void GameOver::Render(RenderContext& rc)
{
	//画像の描画。
	m_spriteRender.Draw(rc);
}