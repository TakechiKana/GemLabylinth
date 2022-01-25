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
	//BGMを削除する。
	//DeleteGO(m_bgm);
}

bool Score::Start()
{
	//画像を読み込む。
	m_spriteRender.Init("Assets/sprite/score/score.dds", WIDHT, 1080);

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

void Score::Update()
{
	if (m_isWaitFadeout) {
		//フェード中じゃなかったら。
		if (!m_fade->IsFade()) {
			NewGO<Title>(0, "title");
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
	ManageState();
	//画像の更新。
	m_spriteRender.Update();
}

void Score::ClearFont()
{
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"現在時刻　　　AM %02d:%02d", m_hour, m_minutes);
	//取得個数
	//表示するテキストを設定。
	m_time.SetText(wcsbuf);
	//フォントの位置を設定。
	m_time.SetPosition(Vector3(0.0f, 300, 0.0f));
	//フォントの大きさを設定。
	m_time.SetScale(2.0f);

	swprintf_s(wcsbuf, 256, L"使ったアイテム数　　　　%d", m_item);
	//取得個数
	//表示するテキストを設定。
	m_useitem.SetText(wcsbuf);
	//フォントの位置を設定。
	m_useitem.SetPosition(Vector3(0.0f, 100, 0.0f));
	//フォントの大きさを設定。
	m_useitem.SetScale(2.0f);
	swprintf_s(wcsbuf, 256, L"Game Clear");
	//取得個数
	//表示するテキストを設定。
	m_gameclear.SetText(wcsbuf);
	//フォントの位置を設定。
	m_gameclear.SetPosition(Vector3(0.0f, -300, 0.0f));
	//フォントの大きさを設定。
	m_gameclear.SetScale(2.5f);
	if (m_hour >= 4)
	{
		swprintf_s(wcsbuf, 256, L"いつもと同じ時間に起きた！朝ごはん食べよう。");
		//取得個数
		//表示するテキストを設定。
		m_comment1.SetText(wcsbuf);
		//フォントの位置を設定。
		m_comment1.SetPosition(Vector3(0.0f, -100, 0.0f));
		//フォントの大きさを設定。
		m_comment1.SetScale(1.5f);
	}
	else
	{
		swprintf_s(wcsbuf, 256, L"今日はいつもより早く起きすぎちゃった。予習しようかな？");
		//取得個数
		//表示するテキストを設定。
		m_comment2.SetText(wcsbuf);
		//フォントの位置を設定。
		m_comment2.SetPosition(Vector3(0.0f, -100, 0.0f));
		//フォントの大きさを設定。
		m_comment2.SetScale(1.5f);
	}
}

void Score::GameOverFont()
{
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"現在時刻　　　AM 05:00");
	//取得個数
	//表示するテキストを設定。
	m_time.SetText(wcsbuf);
	//フォントの位置を設定。
	m_time.SetPosition(Vector3(0.0f, 300, 0.0f));
	//フォントの大きさを設定。
	m_time.SetScale(2.0f);

	swprintf_s(wcsbuf, 256, L"使ったアイテム数　　　　%d", m_item);
	//取得個数
	//表示するテキストを設定。
	m_useitem.SetText(wcsbuf);
	//フォントの位置を設定。
	m_useitem.SetPosition(Vector3(0.0f, 100, 0.0f));
	//フォントの大きさを設定。
	m_useitem.SetScale(2.0f);
	swprintf_s(wcsbuf, 256, L"どこまでいっても迷宮。寝ても覚めても迷宮…。");
	//取得個数
	//表示するテキストを設定。
	m_gameover.SetText(wcsbuf);
	//フォントの位置を設定。
	m_gameover.SetPosition(Vector3(0.0f, -300, 0.0f));
	//フォントの大きさを設定。
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
	//画像の描画。
	m_spriteRender.Draw(rc);
}