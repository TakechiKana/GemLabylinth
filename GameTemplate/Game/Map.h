#pragma once

class Player;
class Gem;

class Map: public IGameObject
{
public:
	Map() {};
	~Map() {};
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	const bool WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 worldPosition, Vector3& mapPosition);
	SpriteRender m_mapSprite;		//�}�b�v
	SpriteRender m_playerSprite;	//�v���C���[
	SpriteRender m_gemSprite;		//�z�E�Z�L

	Vector3 gemsPos;

	Player* m_player = nullptr;
	//std::vector<Gem*>  m_gems;
	bool m_isImage = false;
};

