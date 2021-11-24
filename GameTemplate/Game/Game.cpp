#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "Player.h"
#include "GameCamera.h"
#include "Item.h"
#include "ItemHeart.h"
#include "ItemDash.h"
#include "ItemMagic.h"
#include "ItemPunchUp.h"
#include "Enemy.h"


bool Game::Start()
{

	m_player = FindGO<Player>("player");

	m_levelRender.Init("Assets/modelData/stage/stage.tkl", [&](LevelObjectData& objData) {
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
			playerPos = m_player->GetPosition();

			//falseにすると、レベルの方でモデルが読み込まれて配置される。
			return true;
		}

		if (objData.EqualObjectName(L"enemy") == true) {

			//プレイヤーのオブジェクトを作る。
			m_enemy = NewGO<Enemy>(0, "enemy");
			m_enemy->SetPosition(objData.position);

			//falseにすると、レベルの方でモデルが読み込まれて配置される。
			return true;
		}
		if (objData.EqualObjectName(L"heart") == true) {

			//プレイヤーのオブジェクトを作る。
			m_heart = NewGO<ItemHeart>(0, "heart");
			m_heart->SetPosition(objData.position);
			//falseにすると、レベルの方でモデルが読み込まれて配置される。
			return true;
		}

		if (objData.EqualObjectName(L"glove") == true) {

			//プレイヤーのオブジェクトを作る。
			m_punchUp = NewGO<ItemPunchUp>(0, "punchup");
			m_punchUp->SetPosition(objData.position);
			//falseにすると、レベルの方でモデルが読み込まれて配置される。
			return true;
		}

		if (objData.EqualObjectName(L"boots") == true) {

			//プレイヤーのオブジェクトを作る。
			m_dash = NewGO<ItemDash>(0, "dash");
			m_dash->SetPosition(objData.position);
			//falseにすると、レベルの方でモデルが読み込まれて配置される。
			return true;
		}

		if (objData.EqualObjectName(L"basket") == true) {

			//プレイヤーのオブジェクトを作る。
			m_magic = NewGO<ItemMagic>(0, "magic");
			m_magic->SetPosition(objData.position);
			//falseにすると、レベルの方でモデルが読み込まれて配置される。
			return true;
		}
		return true;
	});
		
	//ゲームカメラのオブジェクトを作る。
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");

	//m_skyCube = NewGO<SkyCube>(0, "skycube");
	//m_skyCube->SetScale(Vector3::One * 3000.0f);
	//m_skyCube->SetType(enSkyCubeType_SunriseToon);

	/*m_backGround = NewGO<BackGround>(0, "background");
	m_player = NewGO<Player>(0, "player");
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");
	*/

	return true;
}


void Game::Update()
{
	/*m_modelRender.Update();*/
}

void Game::Render(RenderContext& rc)
{
	/*m_modelRender.Draw(rc);*/
}