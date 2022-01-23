#pragma once

//�N���X�錾
class Fade;
//class SoundSource;

class Score :public IGameObject
{
public:
	Score() {};
	~Score();
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
	/// ���Ԃ̐ݒ�(��)�B
	/// </summary>
	void SetHour(int hour)
	{
		m_hour = hour;
	}
	/// <summary>
	/// ���Ԃ̐ݒ�(��)�B
	/// </summary>
	void SetMinutes(int minutes)
	{
		m_minutes = minutes;
	}
	/// <summary>
	/// �����\���B
	/// </summary>
	void Font();
	/// <summary>
	/// �`�揈���B
	/// </summary>
	/// <param name="rc">�����_�[�R���e�L�X�g�B</param>
	void Render(RenderContext& rc);
private:
	SpriteRender			m_spriteRender;					//�^�C�g���̉摜�B
	SoundSource*			m_bgm = nullptr;				//BGM
	FontRender				m_time;
	FontRender				m_useitem;
	FontRender				m_comment1;
	FontRender				m_comment2;
	FontRender				m_gameover;
	Fade*					m_fade = nullptr;				//�t�F�[�h�B
	bool					m_isWaitFadeout = false;
	int						m_hour;
	int						m_minutes;
	int						m_leftgem;
	int						m_item;
};