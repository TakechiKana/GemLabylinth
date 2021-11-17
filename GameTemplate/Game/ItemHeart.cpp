#include "stdafx.h"
#include "ItemHeart.h"
#include "Item.h"
#include "Player.h"

bool ItemHeart::Start()
{
	m_modelRender.Init("Assets/modelData/item/heart.tkm");
	m_modelRender.SetScale({0.3f,0.3f,0.3f});
	m_modelRender.Update();
	return true;
}

void ItemHeart::Update()
{
	m_item->GetItem(m_position);
	HeartGetAndUse();
	m_modelRender.Update();
}
void ItemHeart::HeartGetAndUse()
{
	//アイテムをゲットした判定
	if (m_item->GetItem(m_position) == true) {
		//GetItem関数の戻り値を代入
		getFlag = m_item->GetItem(m_position);
		m_item->GetItemCount(getFlag, count);
		SetCount(m_item->GetItemCount(getFlag, count));
		m_item->PlayEffectandSE();
		DeleteGO(this);
	}
	//アイテムを使った判定
	if (g_pad[0]->IsTrigger(enButtonX))
	{
		if (useFlag == true) {
			m_item->ItemUse(useFlag, count);
			SetCount(m_item->ItemUse(getFlag, count));
		}
	}
}

void ItemHeart::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}