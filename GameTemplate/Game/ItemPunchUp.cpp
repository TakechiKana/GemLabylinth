#include "stdafx.h"
#include "ItemPunchUp.h"
#include "Item.h"
#include "Player.h"

bool ItemPunchUp::Start()
{
	m_modelRender.Init("Assets/modelData/item/heart.tkm");
	m_modelRender.SetScale({ 0.3f,0.3f,0.3f });
	m_modelRender.Update();
	return true;
}

void ItemPunchUp::Update()
{
	m_item.GetItem(m_position);
	PunchUpGet();
	m_modelRender.Update();
}

void ItemPunchUp::PunchUpGet()
{
	//アイテムをゲットした判定
	if (m_item.GetItem(m_position) == true) {
		DeleteGO(this);
	}
}

void ItemPunchUp::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}