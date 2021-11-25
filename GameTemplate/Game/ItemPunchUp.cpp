#include "stdafx.h"
#include "ItemPunchUp.h"
#include "Item.h"
#include "Player.h"

bool ItemPunchUp::Start()
{
	m_modelRender.Init("Assets/modelData/item/heart.tkm");
	m_modelRender.SetScale({ 0.3f,0.3f,0.3f });
	m_modelRender.Update();
	m_player = FindGO<Player>("player");
	return true;
}

void ItemPunchUp::Update()
{
	m_item.GetItem(m_position);
	PunchUpGet();
	Rotation();
	m_modelRender.Update();
}

void ItemPunchUp::PunchUpGet()
{
	//ÉAÉCÉeÉÄÇÉQÉbÉgÇµÇΩîªíË
	if (m_item.GetItem(m_position) == true) {
		m_player->GetPunchUpCount();
		DeleteGO(this);
	}
}

void ItemPunchUp::Rotation()
{
	m_rotation.AddRotationDegY(2.0f);

	//äGï`Ç´Ç≥ÇÒÇ…âÒì]Çã≥Ç¶ÇÈÅB
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale({ 0.3f,0.3f,0.3f });
}

void ItemPunchUp::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}