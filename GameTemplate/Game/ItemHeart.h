#pragma once

#include "Item.h"

class Item;
class Player;

class ItemHeart:public IGameObject
{
	
public:
	ItemHeart(){}
	~ItemHeart() {}
	bool Start();
	int ServeCount(){
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
	void HeartGet();
	void Render(RenderContext& rc);
private:
	ModelRender m_modelRender;
	Vector3 m_position;
	Item m_item;
	Player* m_player;
	int count = 0;
	bool getFlag = false;
	bool useFlag = false;
};

