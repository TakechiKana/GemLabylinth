#pragma once

//�N���X�錾
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
	/// �����������B
	/// </summary>
	/// <returns>����������������������Atrue�B</returns>
	bool Start();
	/// <summary>
	/// �X�V�����B
	/// </summary>
	void Update();
	/// <summary>
	/// �Q�[���I�[�o�[�̐ݒ�
	/// </summary>
	void SetGameOver()
	{
		m_scoreState = enScoreState_GameOver;
	}
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
	/// �g�����A�C�e�����̐ݒ�B
	/// </summary>
	void SetUseItem(int item)
	{
		m_item = item;
	}
	/// <summary>
	/// �c�����W�F�����̐ݒ�B
	/// </summary>
	void SetLeftGem(int leftgem)
	{
		m_leftgem = leftgem;
	}
	/// <summary>
	/// �N���A�X�R�A�\���B
	/// </summary>
	void ClearFont();
	/// <summary>
	/// �Q�[���I�[�o�[�X�R�A�\���B
	/// </summary>
	void GameOverFont();
	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	void ManageState();
	/// <summary>
	/// �`�揈���B
	/// </summary>
	/// <param name="rc">�����_�[�R���e�L�X�g�B</param>
	void Render(RenderContext& rc);
private:
	EnScoreState			m_scoreState = enScoreState_Clear;
	SpriteRender			m_spriteRender;					//�^�C�g���̉摜�B
	SoundSource*			m_bgm = nullptr;				//BGM
	FontRender				m_time;
	FontRender				m_useitem;
	FontRender				m_gameclear;
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