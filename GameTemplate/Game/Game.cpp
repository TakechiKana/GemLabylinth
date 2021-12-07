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
			//�z�u���W�A�X�P�[���A��]���擾����B
			m_backGround->SetPosition(objData.position);
			//backGround->scale = objData.scale;
			//backGround->rotation = objData.rotation;
			//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
			return true;

		}

		if (objData.EqualObjectName(L"player") == true) {

			//�v���C���[�̃I�u�W�F�N�g�����B
			m_player = NewGO<Player>(0, "player");
			m_player->SetPosition(objData.position);
			m_player->SetRotation(objData.rotation);

			//false�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ĕz�u�����B
			return true;
		}

		if (objData.ForwardMatchName(L"enemy") == true) {

			//SpawnEnemy();
			//�v���C���[�̃I�u�W�F�N�g�����B
			m_enemy = NewGO<Enemy>(0, "enemy");
			m_enemy->SetPosition(objData.position);

			//false�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ĕz�u�����B
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
		//	//false�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ĕz�u�����B
		//	return true;
		//}
		if (objData.EqualObjectName(L"gem") == true) {

			//�v���C���[�̃I�u�W�F�N�g�����B
			m_gem = NewGO<Gem>(0, "gem");
			m_gem->SetPosition(objData.position);
			//false�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ĕz�u�����B
			return true;
		}
		return true;
	});
		
	//�Q�[���J�����̃I�u�W�F�N�g�����B
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