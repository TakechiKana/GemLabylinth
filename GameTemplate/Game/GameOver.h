#pragma once

//クラス宣言
class Fade;
//class SoundSource;

/// <summary>
/// タイトル。
/// </summary>
class GameOver : public IGameObject
{
public:
	GameOver() {};
	~GameOver();
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
	/// 描画処理。
	/// </summary>
	/// <param name="rc">レンダーコンテキスト。</param>
	void Render(RenderContext& rc);
private:
	SpriteRender			m_spriteRender;					//タイトルの画像。
	SoundSource* m_bgm = nullptr;				//BGM
	Fade* m_fade = nullptr;				//フェード。
	bool					m_isWaitFadeout = false;
};