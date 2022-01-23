#pragma once

class Fade;
class Prologue : public IGameObject
{
public:
	Prologue();
	~Prologue();
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
	SpriteRender			m_spriteRender;					//操作説明画像。
	SoundSource*			m_bgm = nullptr;				//BGM
	Fade*					m_fade = nullptr;				//フェード。
	bool					m_isWaitFadeout = false;
};

