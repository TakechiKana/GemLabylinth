#pragma once
class Fade : public IGameObject
{
public:
	Fade() {};
	~Fade() {};
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	//フェードイン
	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}


	//フェードアウト
	void StartFadeOut()
	{
		m_state = enState_FadeOut;
	}

	//フェード中？
	const bool IsFade() const
	{
		return m_state != enState_Idle;
	}

	//α値
	const float GetAlpha() const
	{
		return m_alpha;
	}

private:
	enum EnState {
		enState_FadeIn,			//フェードイン中。	
		enState_FadeOut,		//フェードアウト中。
		enState_Idle			//アイドル中。
	};

	void AutoFade();

	//状態
	EnState m_state = enState_Idle;
	//画像
	SpriteRender		m_dreamRender;		//タイトル→ゲーム
	SpriteRender		m_gameOverRender;	//ゲーム→ゲームオーバー
	SpriteRender		m_gameClearRender;	//ゲーム→ゲームクリア

	float m_alpha = 0.0f;					//α値　変更すると透明になる
	float m_timer = 0.0f;					//フェードタイマー
};

