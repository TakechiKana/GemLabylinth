#pragma once
#include "Item.h"

class Player;

class Gem : public IGameObject
{
public:
	Gem() {}
	~Gem() {}
	bool Start();
	void Render(RenderContext& rc);

	const Vector3 Getposition() const
	{
		return m_position;
	}

	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	Vector3 ServePosition()
	{
		return m_position;
	}

	/*void GemMap(RenderContext& rc)
	{
		if (m_isStart = false) {
			return;
		}
		m_gemMap.Draw(rc);
	}*/

	void Count();
	void Rotation();
	void Update();

private:
	ModelRender m_modelRender;
	//SpriteRender m_gemMap;
	Vector3 m_position;
	Quaternion m_rotation;
	Item m_item;
	Player* m_player;
	bool m_isStart = false;
};