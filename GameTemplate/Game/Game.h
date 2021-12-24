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
	void Prologue();
	//void ManageState();
	void AlphaValue() 
	{
		switch (m_alphaState) {
		case enAlphaState_FadeOut:
			m_alpha -= 1.0f * g_gameTime->GetFrameDeltaTime();
			if (m_alpha <= 0.0f) {
				m_alpha = 0.0f;
				m_alphaState = enAlphaState_Idle;
			}
			break;
		case enAlphaState_FadeIn:
			m_alpha += 1.0f * g_gameTime->GetFrameDeltaTime();
			if (m_alpha >= 1.0f) {
				m_alpha = 1.0f;
				m_alphaState = enAlphaState_Idle;
			}
			break;
		case enAlphaState_Idle:

			break;
		}
	}
	void PageUp()
	{
		
		m_alphaState = enAlphaState_FadeIn;
		if (m_alphaState = enAlphaState_Idle) {
			m_pageTimer -= GameTime().GetFrameDeltaTime();
			if (m_pageTimer <= 0.0f)
			{

				m_alphaState = enAlphaState_FadeOut;

			}
			m_pro_page += 1;
		}
	}

	EnGameState m_gameState = enGameState_Start;
	EnAlphaState m_alphaState = enAlphaState_Idle;
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
	//bool m_isPrologue = false;

	SpriteRender m_Prologue1;
	SpriteRender m_Prologue2;
	SpriteRender m_Prologue3;
	SpriteRender m_Prologue4;
	SpriteRender m_Prologue5;
	Vector3 playerPos;

	FontRender font1;
};