#pragma once

class Player;
class ItemHeart;
class ItemMagic;
class ItemDash;

class Item
{
public:
	Item();
	~Item() {}

	//ƒAƒCƒeƒ€‚Ì”Ô†æ“¾
	//void SetNumber(const int& num)
	//{
	//	itemNum = num;
	//}

	/*void SpawnItem();*/

	bool GetItem(Vector3 position);

	void PlayEffectandSE(Vector3 position);

private:
	Quaternion m_rotation;
	Vector3 position;
	Player* m_player = nullptr;
	ItemHeart* m_heart;
	ItemMagic* m_magic;
	ItemDash* m_dash;
	/*int itemNum = 0;
	int ran = 0;*/
	int m_state;
	bool m_state1;
	int getCount = 0;
};

