#pragma once

//�N���X�錾
class Fade;
//class SoundSource;

/// <summary>
/// �^�C�g���B
/// </summary>
class GameOver : public IGameObject
{
public:
	GameOver() {};
	~GameOver();
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
	SpriteRender			m_spriteRender;					//�^�C�g���̉摜�B
	SoundSource* m_bgm = nullptr;				//BGM
	Fade* m_fade = nullptr;				//�t�F�[�h�B
	bool					m_isWaitFadeout = false;
};