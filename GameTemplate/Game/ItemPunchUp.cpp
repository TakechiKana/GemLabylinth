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
	m_item->GetItem(m_position);
	PunchUpGetAndUse();
	m_modelRender.Update();
}

void ItemPunchUp::PunchUpGetAndUse()
{
	//アイテムをゲットした判定
	if (m_item->GetItem(m_position) == true) {
		DeleteGO(this);
	}
	//アイテムを使った判定
	if (g_pad[0]->IsTrigger(enButtonB) && m_player->GetDashTimer() <= 0.0f)
	{
	}
}

void ItemPunchUp::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}