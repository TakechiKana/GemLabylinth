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
	//�X�e�[�g
	enum EnGameState {
		enGameState_Start,					//�X�^�[�g
		enGameState_Game,					//�Q�[����
		enGameState_Prologue,					//�v�����[�O�A�������
		enGameState_Event,					//�I���
		enGameState_Score,					//�X�R�A
	};
	enum EnAlphaState {
		enAlphaState_FadeIn,					//prologue�͂��܂�
		enAlphaState_FadeOut,					//prologue�I���
		enAlphaState_Idle,						//�\����
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
	BackGround* m_backGround = nullptr;			//�X�e�[�W
	Player* m_player = nullptr;					//�v���C���[
	GameCamera* m_gameCamera = nullptr;			//�J����
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