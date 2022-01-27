#pragma once
#include "Item.h"

class Player;
class Game;

class Clock : public IGameObject
{
public:
	Clock() {}
	~Clock() {}
	bool Start();
	void Render(RenderContext& rc);

	const Vector3 Getposition() const
	{
		return m_position;
	}

	Vector3 ServePosition()
	{
		return m_position;
	}

	void Count();
	void Rotation();
	void Update();

private:
	ModelRender m_modelRender;
	//SpriteRender m_gemMap;
	Vector3 m_position;
	Quaternion m_rotation;
	Item m_item;
	Player* m_player = nullptr;
	Game* m_game = nullptr;
	bool m_isStart = false;
};