#pragma once

class Fade;
class Prologue : public IGameObject
{
public:
	Prologue();
	~Prologue();
	/// <summary>
	/// �����������B
	/// </summary>
	/// <returns>����������������������Atrue�B</returns>
	bool Start();
	/// <summary>
	/// �X�V�����B
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈���B
	/// </summary>
	/// <param name="rc">�����_�[�R���e�L�X�g�B</param>
	void Render(RenderContext& rc);
private:
	SpriteRender			m_spriteRender;					//��������摜�B
	SoundSource*			m_bgm = nullptr;				//BGM
	Fade*					m_fade = nullptr;				//�t�F�[�h�B
	bool					m_isWaitFadeout = false;
};

