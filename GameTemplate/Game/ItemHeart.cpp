#include "stdafx.h"
#include "ItemHeart.h"
#include "Item.h"
#include "Player.h"

bool ItemHeart::Start()
{
	m_modelRender.Init("Assets/modelData/item/heart.tkm");
	m_modelRender.SetScale({0.3f,0.3f,0.3f});
	m_modelRender.Update();
	m_player = FindGO<Player>("player");

	return true;
}

void ItemHeart::Update()
{
	m_item.GetItem(m_position);
	HeartGet();
	Rotation();
	m_modelRender.Update();
}
void ItemHeart::HeartGet()
{
	//ƒAƒCƒeƒ€‚ğƒQƒbƒg‚µ‚½”»’è
	if (m_item.GetItem(m_position) == true) {
		m_player->GetHeartCount();
		DeleteGO(this);
	}
}
void ItemHeart::Rotation()
{
	m_rotation.AddRotationDegY(2.0f);

	//ŠG•`‚«‚³‚ñ‚É‰ñ“]‚ğ‹³‚¦‚éB
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale({ 0.3f,0.3f,0.3f });
}

void ItemHeart::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}