#pragma once
#include "Item.h"
#include "Score.h"
#include "Level3DRender/LevelRender.h"

class Player;
class BackGround;
class GameCamera;
class ItemDash;
class Enemy;
class Gem;
class Fade;

//class Map;

class Game : public IGameObject
{
public:
	Game() {}
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void GameTimer();
	void Font();
	void GameJudge();
	void LeftGem()
	{
		m_gems -= 1;
	}
	void UseItem()
	{
		m_useItem += 1;
	}
	void SetGameOverFlag()
	{
		m_isGameover = true;
	}
private:
	std::vector< PointLight*> m_lightArray;
	std::vector< SpotLight*> m_sptLightArray;
	std::vector< VolumeSpotLight*> m_volumeSptLightArray;
	//ModelRender m_modelRender;
	LevelRender m_levelRender;
	BackGround* m_backGround = nullptr;			//ステージ
	Player* m_player = nullptr;					//プレイヤー
	GameCamera* m_gameCamera = nullptr;			//カメラ
	ItemDash* m_dash = nullptr;
	Item m_item;
	Score m_score;
	Enemy* m_enemy = nullptr;
	Gem* m_gem = nullptr;
	Fade* m_fade = nullptr;

	float m_timer = 0.0f;

	int m_hour = 0;
	int m_minutes = 0;
	int m_gems = 0;
	int m_useItem = 0;

	bool m_isWaitFadeout = false;
	bool m_isGameover = false;

	Vector3 m_clockPos;
	FontRender m_timeRender;
	FontRender m_gemRender;
	ModelRender m_clockRender;
};