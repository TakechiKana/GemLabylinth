#include "stdafx.h"
#include "Fade.h"

bool Fade::Start()
{
	m_dreamRender.Init("Assets/sprite/fade/fade_dream.dds", 1980.0f, 1080.0f);
	m_gameOverRender.Init("Assets/sprite/fade/fade_gameover.dds", 1980.0f, 1080.0f);
	m_gameClearRender.Init("Assets/sprite/fade/fade_gameclear.dds", 1980.0f, 1080.0f);
	
	m_dreamRender.Update();
	m_gameOverRender.Update();
	m_gameClearRender.Update();
	return true;
}

void Fade::Update()
{

	switch (m_state) {
	case enState_FadeIn:
		m_alpha -= g_gameTime->GetFrameDeltaTime();
		if (m_alpha <= 0.0f) {
			m_alpha = 0.0f;
			m_state = enState_Idle;
		}
		break;
		
	case enState_FadeOut:
		m_alpha += g_gameTime->GetFrameDeltaTime();
		if (m_alpha >= 1.0f) {
			m_state = enState_Idle;
			m_alpha = 1.0f;
		}
		break;
	case enState_Idle:

		break;
	}
}
void Fade::Render(RenderContext& rc)
{
	if (m_alpha > 0.0f) {
		m_dreamRender.SetMulColor({ 1.0f, 1.0f, 1.0f, m_alpha });
		m_dreamRender.Draw(rc);
	}
}