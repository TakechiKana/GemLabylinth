#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
class BackGround;
class GameCamera;
class ItemDash;
class ItemHeart;
class ItemMagic;
class ItemPunchUp;

class Game : public IGameObject
{
public:
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	const Vector3 GetPosition()const
	{
		return playerPos;
	}

private:
	SkyCube m_skyCube;
	LevelRender m_levelRender;
	BackGround* m_backGround = nullptr;			//�X�e�[�W
	Player* m_player = nullptr;					//�v���C���[
	GameCamera* m_gameCamera = nullptr;			//�J����
	ItemDash* m_dash = nullptr;
	ItemHeart* m_heart = nullptr;
	ItemMagic* m_magic = nullptr;
	ItemPunchUp* m_punchUp = nullptr;

	ModelRender m_modelRender;
	Vector3 playerPos;
};//�������

//���肢���܂��I