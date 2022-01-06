#pragma once
class Fade : public IGameObject
{
public:
	Fade() {};
	~Fade() {};
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	//�t�F�[�h�C��
	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}


	//�t�F�[�h�A�E�g
	void StartFadeOut()
	{
		m_state = enState_FadeOut;
	}

	//�t�F�[�h���H
	const bool IsFade() const
	{
		return m_state != enState_Idle;
	}

	//���l
	const float GetAlpha() const
	{
		return m_alpha;
	}

private:
	enum EnState {
		enState_FadeIn,			//�t�F�[�h�C�����B	
		enState_FadeOut,		//�t�F�[�h�A�E�g���B
		enState_Idle			//�A�C�h�����B
	};

	void AutoFade();

	//���
	EnState m_state = enState_Idle;
	//�摜
	SpriteRender		m_dreamRender;		//�^�C�g�����Q�[��
	SpriteRender		m_gameOverRender;	//�Q�[�����Q�[���I�[�o�[
	SpriteRender		m_gameClearRender;	//�Q�[�����Q�[���N���A

	float m_alpha = 0.0f;					//���l�@�ύX����Ɠ����ɂȂ�
	float m_timer = 0.0f;					//�t�F�[�h�^�C�}�[
};

