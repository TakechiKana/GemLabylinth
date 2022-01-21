#include "stdafx.h"
#include "Gem.h"
#include "Player.h"


bool Gem::Start()
{
	//m_gemMap.Init("Assets/sprite/enemy.dds", 35, 35);
	m_modelRender.Init("Assets/modelData/item/gem.tkm");
	//m_modelRender.SetScale({ 10.0f,10.0f,10.0f });
	m_player = FindGO<Player>("player");
	return true;
}

void Gem::Update()
{
	Count();
	Rotation();
	m_modelRender.Update();
}


void Gem::Count()
{
	m_item.GetItem(ServePosition());
	//ÉAÉCÉeÉÄÇÉQÉbÉgÇµÇΩîªíË
	if (m_item.GetItem(ServePosition()) == true) {
		m_player->LeftGemCount();
		DeleteGO(this);
	}
}
void Gem::Rotation()
{
	m_rotation.AddRotationDegY(2.0f);

	//äGï`Ç´Ç≥ÇÒÇ…âÒì]Çã≥Ç¶ÇÈÅB
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale({ 0.3f,0.3f,0.3f });
}

void Gem::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
