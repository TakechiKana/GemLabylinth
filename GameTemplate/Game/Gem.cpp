#include "stdafx.h"
#include "Gem.h"
#include "Player.h"
#include "Game.h"


bool Gem::Start()
{
	m_modelRender.Init("Assets/modelData/item/gem.tkm");
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
	//アイテムをゲットした判定
	if (m_item.GetItem(ServePosition()) == true) {
		m_player->SetLeftGemCount();
		DeleteGO(this);
	}
}
void Gem::Rotation()
{
	m_rotation.AddRotationDegY(2.0f);

	//絵描きさんに回転を教える。
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale({ 0.3f,0.3f,0.3f });
}

void Gem::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
