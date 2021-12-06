#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
class BackGround;
class GameCamera;
class ItemDash;
class ItemHeart;
class ItemMagic;
class ItemPunchUp;
class Enemy;
class Gem;

class Game : public IGameObject
{
public:
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SpawnItem();
	void SpawnEnemy();

	const Vector3 GetPosition()const
	{
		return playerPos;
	}

private:
	SkyCube m_skyCube;
	LevelRender m_levelRender;
	BackGround* m_backGround = nullptr;			//ステージ
	Player* m_player = nullptr;					//プレイヤー
	GameCamera* m_gameCamera = nullptr;			//カメラ
	ItemDash* m_dash = nullptr;
	ItemHeart* m_heart = nullptr;
	ItemMagic* m_magic = nullptr;
	ItemPunchUp* m_punchUp = nullptr;
	Enemy* m_enemy = nullptr;
	Gem* m_gem = nullptr;

	ModelRender m_modelRender;
	Vector3 playerPos;
};//来たわよ

//お願いします！