#pragma once

class Player;
class ItemDash;

class Item
{
public:
	Item();
	~Item() {}

	bool GetItem(Vector3 position);

	void PlayEffectandSE(Vector3 position);

private:
	Quaternion m_rotation;
	Vector3 position;
	Player* m_player = nullptr;
	ItemDash* m_dash;
};

