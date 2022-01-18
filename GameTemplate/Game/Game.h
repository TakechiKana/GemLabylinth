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
//class Map;

class Game : public IGameObject
{
public:
	//ステート
	enum EnGameState {
		enGameState_Start,					//スタート
		enGameState_Game,					//ゲーム中
		enGameState_Prologue,					//プロローグ、操作説明
		enGameState_Event,					//終わり
		enGameState_Score,					//スコア
	};
	enum EnAlphaState {
		enAlphaState_FadeIn,					//prologueはじまり
		enAlphaState_FadeOut,					//prologue終わり
		enAlphaState_Idle,						//表示中
	};

public:
	Game() {}
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	const EnGameState GetState() {
		return m_gameState;
	}


	const float GetTimer()const
	{
		return m_fadeTimer;
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

	//void ManageState();

	EnGameState m_gameState = enGameState_Game;
	EnAlphaState m_alphaState = enAlphaState_Idle;
	SkyCube m_skyCube;
	std::vector< PointLight*> m_lightArray;
	std::vector< SpotLight*> m_sptLightArray;
	std::vector< VolumeSpotLight*> m_volumeSptLightArray;
	LevelRender m_levelRender;
	BackGround* m_backGround = nullptr;			//ステージ
	Player* m_player = nullptr;					//プレイヤー
	GameCamera* m_gameCamera = nullptr;			//カメラ
	ItemDash* m_dash = nullptr;
	ItemMagic* m_magic = nullptr;
	Enemy* m_enemy = nullptr;
	Gem* m_gem = nullptr;
	Fade* m_fade = nullptr;
	//Map* m_map = nullptr;

	int itemNum = 0;
	bool m_cameraFlag = false;
	bool m_fadeFlag = false;
	float m_fadeTimer = 0.0f;
	float m_pageTimer = 4.0f;
	float m_alpha = 0.0f;
	int m_pro_page = 1;
	bool m_pageFlag = false;
	bool m_isWaitFadeout = false;

	Vector3 m_gemPos;
	Vector3 m_dashPos;

	FontRender font1;
};