#include "stdafx.h"
#include "ItemDash.h"
#include "Player.h"


bool ItemDash::Start()
{
	m_modelRender.Init("Assets/modelData/item/heart.tkm");
	return true;
}

void ItemDash::Update()
{
	//kari= m_player->GetPosition();

	m_item.GetItem(ServePosition());
	DashGetAndUse();
	m_modelRender.Update();
	Rotation();
}


void ItemDash::DashGetAndUse()
{
	//アイテムをゲットした判定
	if (m_item.GetItem(ServePosition()) == true) {
		//GetItem関数の戻り値を代入
		getFlag = m_item.GetItem(ServePosition());
		m_item.GetItemCount(getFlag, count);
		SetCount(m_item.GetItemCount(getFlag, count));
		DeleteGO(this);
	}
	//アイテムを使った判定
	if (g_pad[0]->IsTrigger(enButtonB)&& m_player->GetDashTimer()<=0.0f && count>=0)
	{
		if (useFlag == true) {
			m_item.ItemUse(useFlag,count);
			SetCount(m_item.ItemUse(getFlag, count));
		}
	}
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