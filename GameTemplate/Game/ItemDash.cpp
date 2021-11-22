#include "stdafx.h"
#include "ItemDash.h"
#include "Player.h"


bool ItemDash::Start()
{
	m_modelRender.Init("Assets/modelData/item/boots_kari.tkm");
	m_player = FindGO<Player>("player");
	return true;
}

void ItemDash::Update()
{
	DashGetAndUse();
	Rotation();
	m_modelRender.Update();
}


void ItemDash::DashGetAndUse()
{
	m_item.GetItem(ServePosition());
	//アイテムをゲットした判定
	if (m_item.GetItem(ServePosition()) == true) {
		count += 1;
		DeleteGO(this);
	}

	//アイテムを使った判定
	//if (m_player->GetState() == true)
	//{ 
	//		count -= 1;

	//	/*if (m_player->GetDashTimer() >= 3.9f) {
	//		m_item.GetItemCount(1, count);
	//	}
	//	SetCount(m_item.GetItemCount(1, count));*/
	//}
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