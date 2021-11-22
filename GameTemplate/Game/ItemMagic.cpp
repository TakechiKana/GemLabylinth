#include "stdafx.h"
#include "ItemMagic.h"
#include "Item.h"
#include "Player.h"

bool ItemMagic::Start()
{
	m_modelRender.Init("Assets/modelData/item/heart.tkm");
	m_modelRender.SetScale({ 0.3f,0.3f,0.3f });	
	return true;
}

void ItemMagic::Update()
{
	m_item->GetItem(m_position);
	MagicGetAndUse();
	m_modelRender.Update();
}

void ItemMagic::MagicGetAndUse()
{
	//アイテムをゲットした判定
	if (m_item->GetItem(m_position) == true) {
		if (count < 3) {
			DeleteGO(this);
		}
	}
	//アイテムを使った判定
	if (g_pad[0]->IsTrigger(enButtonB) && m_player->GetDashTimer() <= 0.0f)
	{
	}
}

void ItemMagic::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}