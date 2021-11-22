#pragma once

class Player;
class ItemHeart;
class ItemPunchUp;
class ItemMagic;
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
	ItemHeart* m_heart;
	ItemPunchUp* m_punchUp;
	ItemMagic* m_magic;
	ItemDash* m_dash;
	int m_state;
	bool m_state1;
	int getCount = 0;
};

