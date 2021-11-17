#pragma once
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
	void Update();
	void PunchUpGetAndUse();
	void Render(RenderContext& rc);
private:
	ModelRender m_modelRender;
	Vector3 m_position;
	Item* m_item = nullptr;
	Player* m_player = nullptr;
	int count = 0;
	bool getFlag = false;
	bool useFlag = false;
};

