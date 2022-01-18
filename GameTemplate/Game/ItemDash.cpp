#include "stdafx.h"
#include "ItemDash.h"
#include "Player.h"


bool ItemDash::Start()
{
	m_modelRender.Init("Assets/modelData/item/wings.tkm");
	m_modelRender.SetScale({ 1.4f,1.4f,1.4f });
	m_player = FindGO<Player>("player");
	return true;
}

void ItemDash::Update()
{
	DashGet();
	Rotation();
	m_modelRender.Update();
}


void ItemDash::DashGet()
{
	m_item.GetItem(ServePosition());
	//アイテムをゲットした判定
	if (m_item.GetItem(ServePosition()) == false) {
		return;
	}
	m_player->GetDashCount();
	DeleteGO(this);
}
void ItemDash::Rotation()
{
	m_rotation.AddRotationDegY(2.0f);

	//絵描きさんに回転を教える。
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale({ 0.3f,0.3f,0.3f });
}

void ItemDash::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}