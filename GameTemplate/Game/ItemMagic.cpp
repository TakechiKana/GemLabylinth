#include "stdafx.h"
#include "ItemMagic.h"
#include "Item.h"
#include "Player.h"

bool ItemMagic::Start()
{
	m_modelRender.Init("Assets/modelData/item/heart.tkm");
	m_modelRender.SetScale({ 0.3f,0.3f,0.3f });	
	m_player = FindGO<Player>("player");

	return true;
}

void ItemMagic::Update()
{
	m_item.GetItem(m_position);
	MagicGet();
	Rotation();
	m_modelRender.Update();
}

void ItemMagic::MagicGet()
{
	//ÉAÉCÉeÉÄÇÉQÉbÉgÇµÇΩîªíË
	if (m_item.GetItem(m_position) == true) {
		m_player->GetMagicCount();
		DeleteGO(this);
	}
}

void ItemMagic::Rotation()
{
	m_rotation.AddRotationDegY(2.0f);

	//äGï`Ç´Ç≥ÇÒÇ…âÒì]Çã≥Ç¶ÇÈÅB
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale({ 0.3f,0.3f,0.3f });
}

void ItemMagic::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}