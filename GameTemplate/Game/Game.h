#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
class BackGround;
class GameCamera;
class ItemDash;
class ItemHeart;
class ItemMagic;
class Enemy;
class Gem;
class Fade;
class LightModel;
//class Map;

class Game : public IGameObject
{
public:
	Game() {}
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	const bool GetCameraFlag()const
	{
		return m_cameraFlag;
	}

	void SetCameraFlag(bool flag)
	{
		m_cameraFlag = flag;
	}

private:
	std::vector< PointLight*> m_lightArray;
	std::vector< SpotLight*> m_sptLightArray;
	std::vector< VolumeSpotLight*> m_volumeSptLightArray;
	ModelRender m_modelRender;
	LevelRender m_levelRender;
	BackGround* m_backGround = nullptr;			//ステージ
	Player* m_player = nullptr;					//プレイヤー
	GameCamera* m_gameCamera = nullptr;			//カメラ
	ItemDash* m_dash = nullptr;
	ItemMagic* m_magic = nullptr;
	Enemy* m_enemy = nullptr;
	Gem* m_gem = nullptr;
	Fade* m_fade = nullptr;
	LightModel* m_lightmodel = nullptr;
	//Map* m_map = nullptr;

	int itemNum = 0;
	bool m_cameraFlag = true;
	bool m_fadeFlag = false;

	bool m_pageFlag = false;
	bool m_isWaitFadeout = false;

	Vector3 m_gemPos;
	Vector3 m_dashPos;

	FontRender font1;
};