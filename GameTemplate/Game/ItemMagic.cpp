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
	//�A�C�e�����Q�b�g��������
	if (m_item->GetItem(m_position) == true) {
		if (count < 3) {
			DeleteGO(this);
		}
	}
	//�A�C�e�����g��������
	if (g_pad[0]->IsTrigger(enButtonB) && m_player->GetDashTimer() <= 0.0f)
	{
	}
}

void ItemMagic::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}