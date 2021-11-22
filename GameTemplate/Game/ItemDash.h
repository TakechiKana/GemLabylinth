#pragma once

#include "Item.h"

class Item;
class Player;

class ItemDash :public IGameObject
{
public:
	ItemDash() {}
	~ItemDash() {}
	bool Start();
	const int ServeCount() const{
		return count;
	}
	void SetCount() 
	{
		count -= 1;
	}

	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	Vector3 ServePosition ()
	{
		return m_position;
	}

	void Dash();
	void DashGetAndUse();
	void Rotation();
	void Update();
	void Render(RenderContext& rc);
private:
	ModelRender m_modelRender;
	Vector3 m_position;
	Quaternion m_rotation;
	Item m_item;
	Player* m_player;
	int count = 0;
	int kari = 0;
};

