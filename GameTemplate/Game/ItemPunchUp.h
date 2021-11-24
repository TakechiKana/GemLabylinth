#pragma once

#include "Item.h"

class Item;
class Player;

class ItemPunchUp :public IGameObject
{
public:
	ItemPunchUp() {}
	~ItemPunchUp(){}
	bool Start();
	int ServeCount() {
		return count;
	}
	int SetCount(int n)
	{
		count = n;
		return count;
	}
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	void UseCount()
	{
		count -= 1;
	}
	bool IsCanUse()
	{
		if (count > 0) {
			return true;
		}
	}
	void Update();
	void PunchUpGet();
	void Render(RenderContext& rc);
private:
	ModelRender m_modelRender;
	Vector3 m_position;
	Item m_item;
	Player* m_player = nullptr;
	int count = 0;
	bool getFlag = false;
	bool useFlag = false;
};

