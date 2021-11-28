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

	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

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