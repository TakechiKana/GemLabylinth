#pragma once
#include "Item.h"

class Player;

class Gem : public IGameObject
{
public:
	Gem() {}
	~Gem() {}
	bool Start();

	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	Vector3 ServePosition()
	{
		return m_position;
	}
	void Count();
	void Rotation();
	void Update();
	void Render(RenderContext& rc);
private:
	ModelRender m_modelRender;
	Vector3 m_position;
	Quaternion m_rotation;
	Item m_item;
	Player* m_player;
};