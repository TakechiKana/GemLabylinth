#pragma once

class Player;
class ItemDash;

class Event : public IGameObject
{
public:
	Event();
	~Event() {}

	bool GetEventPoint(Vector3 position);

	void PlayEffectandSE(Vector3 position);

private:
	Vector3 m_door1open;
	Vector3 m_door1close;
	Vector3 m_door2open;
	Player* m_player = nullptr;
};

