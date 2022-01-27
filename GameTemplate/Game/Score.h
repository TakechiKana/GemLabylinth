#pragma once

//クラス宣言
class Fade;
//class SoundSource;

class Score :public IGameObject
{
public:
	enum EnScoreState {
		enScoreState_Clear,
		enScoreState_GameOver
	};

public:
	Score() {};
	~Score();
	/// <summary>
	/// 初期化処理。
	/// </summary>
	/// <returns>初期化処理が完了したら、true。</returns>
	bool Start();
	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update();
	/// <summary>
	/// ゲームオーバーの設定
	/// </summary>
	void SetGameOver()
	{
		m_scoreState = enScoreState_GameOver;
	}
	/// <summary>
	/// 時間の設定(時)。
	/// </summary>
	void SetHour(int hour)
	{
		m_hour = hour;
	}
	/// <summary>
	/// 時間の設定(分)。
	/// </summary>
	void SetMinutes(int minutes)
	{
		m_minutes = minutes;
	}
	/// <summary>
	/// 使ったアイテム数の設定。
	/// </summary>
	void SetUseItem(int item)
	{
		m_item = item;
	}
	/// <summary>
	/// 残ったジェム数の設定。
	/// </summary>
	void SetLeftGem(int leftgem)
	{
		m_leftgem = leftgem;
	}
	/// <summary>
	/// クリアスコア表示。
	/// </summary>
	void ClearFont();
	/// <summary>
	/// ゲームオーバースコア表示。
	/// </summary>
	void GameOverFont();
	/// <summary>
	/// ステート管理
	/// </summary>
	void ManageState();
	/// <summary>
	/// 描画処理。
	/// </summary>
	/// <param name="rc">レンダーコンテキスト。</param>
	void Render(RenderContext& rc);
private:
	EnScoreState			m_scoreState = enScoreState_Clear;
	SpriteRender			m_spriteRender;					//タイトルの画像。
	SoundSource*			m_bgm = nullptr;				//BGM
	FontRender				m_time;
	FontRender				m_useitem;
	FontRender				m_gameclear;
	FontRender				m_comment1;
	FontRender				m_comment2;
	FontRender				m_gameover;
	Fade*					m_fade = nullptr;				//フェード。
	bool					m_isWaitFadeout = false;
	int						m_hour;
	int						m_minutes;
	int						m_leftgem;
	int						m_item;
};