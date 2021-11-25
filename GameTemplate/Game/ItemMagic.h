#pragma once

#include "Item.h"

class Item;
class Player;

class ItemMagic :public IGameObject
{
public:
	ItemMagic() {}
	~ItemMagic() {}
	bool Start();
	//int ServeCount() {
	//	return count;
	//}
	//int SetCount(int n)
	//{
	//	count = n;
	//	return count;
	//}
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	//void UseCount()
	//{
	//	count -= 1;
	//}
	//bool IsCanUse()
	//{
	//	if (count > 0) {
	//		return true;
	//	}
	//}
	void Update();
	void Rotation();
	void MagicGet();
	void Render(RenderContext& rc);
private:
	ModelRender m_modelRender;
	Vector3 m_position;
	Quaternion m_rotation;
	Item m_item;
	Player* m_player;
};