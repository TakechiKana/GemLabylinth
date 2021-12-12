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

class Game : public IGameObject
{
public:
	//�X�e�[�g
	enum EnGameState {
		enGameState_Start,					//�X�^�[�g
		enGameState_tuto,					//����
		enGameState_Game,					//�Q�[����
		enGameState_End,					//�I���
		enGameState_Score,					//�X�R�A
	};

public:
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SpawnItem();
	void SpawnEnemy();

	const EnGameState Get() {
		return m_gameState;
	}

	const Vector3 GetPosition()const
	{
		return playerPos;
	}

private:
	EnGameState m_gameState = enGameState_Start;
	SkyCube m_skyCube;
	LevelRender m_levelRender;
	BackGround* m_backGround = nullptr;			//�X�e�[�W
	Player* m_player = nullptr;					//�v���C���[
	GameCamera* m_gameCamera = nullptr;			//�J����
	ItemDash* m_dash = nullptr;
	ItemHeart* m_heart = nullptr;
	ItemMagic* m_magic = nullptr;
	Enemy* m_enemy = nullptr;
	Gem* m_gem = nullptr;

	int itemNum = 0;

	ModelRender m_modelRender;
	Vector3 playerPos;
};//�������

//���肢���܂��I