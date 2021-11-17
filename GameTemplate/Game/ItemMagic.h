#pragma once
class Item;
class Player;

class ItemMagic :public IGameObject
{
public:
	ItemMagic() {}
	~ItemMagic() {}
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
	void Magic();
	void Update();
	void MagicGetAndUse();
	void Render(RenderContext& rc);
private:
	ModelRender m_modelRender;
	Vector3 m_position;
	Item* m_item;
	Player* m_player;
	int count = 0;
	bool getFlag = false;
	bool useFlag = false;
};