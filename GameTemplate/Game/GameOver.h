#pragma once

//�N���X�錾
class Fade;
//class SoundSource;
class Score;

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
	/// <summary>
	/// �g�����A�C�e�����̐ݒ�B
	/// </summary>
	void SetUseItem(int item)
	{
		m_useitem = item;
	}
private:
	SpriteRender			m_spriteRender;					//�^�C�g���̉摜�B
	SoundSource*			m_bgm = nullptr;				//BGM
	Fade*					m_fade = nullptr;				//�t�F�[�h�B
	Score*					m_score = nullptr;				//�X�R�A�B
	bool					m_isWaitFadeout = false;		//�t�F�[�h���H
	int						m_useitem = 0;					//�g�����A�C�e����
};