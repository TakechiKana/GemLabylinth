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
class Map;

class Game : public IGameObject
{
public:
	//ステート
	enum EnGameState {
		enGameState_Start,					//スタート
		enGameState_tuto,					//説明
		enGameState_Game,					//ゲーム中
		enGameState_End,					//終わり
		enGameState_Score,					//スコア
	};

public:
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	const EnGameState GetState() {
		return m_gameState;
	}

	const Vector3 GetPosition()const
	{
		return playerPos;
	}

	const bool GetCameraFlag()const
	{
		return m_cameraFlag;
	}

	void SetCameraFlag(bool flag)
	{
		m_cameraFlag = flag;
	}

private:
	void ProcessState();

	EnGameState m_gameState = enGameState_Start;
	SkyCube m_skyCube;
	PointLight* m_light;
	LevelRender m_levelRender;
	BackGround* m_backGround = nullptr;			//ステージ
	Player* m_player = nullptr;					//プレイヤー
	GameCamera* m_gameCamera = nullptr;			//カメラ
	ItemDash* m_dash = nullptr;
	ItemMagic* m_magic = nullptr;
	Enemy* m_enemy = nullptr;
	Gem* m_gem = nullptr;
	Map* m_map = nullptr;

	int itemNum = 0;
	bool m_cameraFlag = false;

	ModelRender m_modelRender;
	Vector3 playerPos;
};