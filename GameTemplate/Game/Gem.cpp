#include "stdafx.h"
#include "Gem.h"
#include "Player.h"


bool Gem::Start()
{
	m_modelRender.Init("Assets/modelData/item/gem_kari.tkm");
	m_player = FindGO<Player>("player");
	return true;
}

void Gem::Update()
{
	Count();
	Rotation();
	m_modelRender.Update();
}


void Gem::Count()
{
	m_item.GetItem(ServePosition());
	//�A�C�e�����Q�b�g��������
	if (m_item.GetItem(ServePosition()) == true) {
		m_player->GetGemCount();
		DeleteGO(this);
	}
}
void Gem::Rotation()
{
	m_rotation.AddRotationDegY(2.0f);

	//�G�`������ɉ�]��������B
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale({ 0.3f,0.3f,0.3f });
}

void Gem::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}