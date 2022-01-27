#include "stdafx.h"
#include "Clock.h"
#include "Player.h"
#include "Game.h"


bool Clock::Start()
{
	m_modelRender.Init("Assets/modelData/item/clock.tkm");
	m_modelRender.SetPosition(Vector3{ 0.0f,30.0f,0.0f });
	m_player = FindGO<Player>("player");
	m_game = FindGO<Game>("game");
	return true;
}

void Clock::Update()
{
	Count();
	Rotation();
	m_modelRender.Update();
}


void Clock::Count()
{
	m_item.GetItem(ServePosition());
	//ÉAÉCÉeÉÄÇÉQÉbÉgÇµÇΩîªíË
	if (m_item.GetItem(ServePosition()) == true) {
		m_game->SetGameClearFlag();
		DeleteGO(this);
	}
}
void Clock::Rotation()
{
	m_rotation.AddRotationDegY(2.0f);

	//äGï`Ç´Ç≥ÇÒÇ…âÒì]Çã≥Ç¶ÇÈÅB
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale({ 0.3f,0.3f,0.3f });
}

void Clock::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
