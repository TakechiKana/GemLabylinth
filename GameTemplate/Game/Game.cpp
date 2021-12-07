#include<random>
#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "Player.h"
#include "GameCamera.h"
#include "Item.h"
#include "ItemHeart.h"
#include "ItemDash.h"
#include "ItemMagic.h"
#include "Enemy.h"
#include "Gem.h"


bool Game::Start()
{

	m_player = FindGO<Player>("player");

	m_levelRender.Init("Assets/modelData/stage/stage2.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"stage") == true) {

			m_backGround = NewGO<BackGround>(0);
			//配置座標、スケール、回転を取得する。
			m_backGround->SetPosition(objData.position);
			//backGround->scale = objData.scale;
			//backGround->rotation = objData.rotation;
			//trueにすると、レベルの方でモデルが読み込まれない。
			return true;

		}

		if (objData.EqualObjectName(L"player") == true) {

			//プレイヤーのオブジェクトを作る。
			m_player = NewGO<Player>(0, "player");
			m_player->SetPosition(objData.position);
			m_player->SetRotation(objData.rotation);

			//falseにすると、レベルの方でモデルが読み込まれて配置される。
			return true;
		}

		if (objData.ForwardMatchName(L"enemy") == true) {

			//SpawnEnemy();
			//プレイヤーのオブジェクトを作る。
			m_enemy = NewGO<Enemy>(0, "enemy");
			m_enemy->SetPosition(objData.position);

			//falseにすると、レベルの方でモデルが読み込まれて配置される。
			return true;
		}
		//if (objData.ForwardMatchName(L"item") == true) {

		//	itemNum = objData.number;
		//	for (int i = 1; i++; i < 11)
		//	{
		//		int ran = 0;
		//		ran = rand() % 2;
		//		if (i == itemNum) 
		//		{
		//			if (ran == 0)
		//			{
		//				m_dash = NewGO<ItemDash>(0, "dash");
		//				m_dash->SetPosition(objData.position);
		//			}
		//			else
		//			{
		//				m_magic = NewGO<ItemMagic>(0, "magic");
		//				m_magic->SetPosition(objData.position);
		//			}
		//		}
		//	}
		//	//falseにすると、レベルの方でモデルが読み込まれて配置される。
		//	return true;
		//}
		if (objData.EqualObjectName(L"gem") == true) {

			//プレイヤーのオブジェクトを作る。
			m_gem = NewGO<Gem>(0, "gem");
			m_gem->SetPosition(objData.position);
			//falseにすると、レベルの方でモデルが読み込まれて配置される。
			return true;
		}
		return true;
	});
		
	//ゲームカメラのオブジェクトを作る。
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");

	return true;
}


void Game::SpawnItem()
{

}
//void Game::SpawnEnemy()
//{
//
//}


void Game::Update()
{
	
}

void Game::Render(RenderContext& rc)
{

}