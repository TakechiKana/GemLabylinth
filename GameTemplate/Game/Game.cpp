#include<random>
#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "Player.h"
#include "GameCamera.h"
#include "Item.h"
#include "ItemDash.h"
#include "ItemMagic.h"
#include "Enemy.h"
#include "Gem.h"
#include "Fade.h"
//#include "Map.h"


bool Game::Start()
{

	m_player = FindGO<Player>("player");

	m_Prologue1.Init("Assets/sprite/prologue/p1.dds", 1980.0f, 1080.0f);
	m_Prologue2.Init("Assets/sprite/prologue/p2.dds", 1980.0f, 1080.0f);
	m_Prologue3.Init("Assets/sprite/prologue/p3.dds", 1980.0f, 1080.0f);
	m_Prologue4.Init("Assets/sprite/prologue/p4.dds", 1980.0f, 1080.0f);
	m_Prologue5.Init("Assets/sprite/prologue/p5.dds", 1980.0f, 1080.0f);

	m_fade = NewGO<Fade>(0, "fade");


	g_renderingEngine->SetSceneMiddleGray(0.1f);
	g_renderingEngine->SetAmbient({ 0.1f, 0.1f, 0.1f });

	m_levelRender.Init("Assets/modelData/stage/stage2.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"stage") == true) {

			m_backGround = NewGO<BackGround>(0);
			//配置座標、スケール、回転を取得する。
			m_backGround->SetPosition(objData.position);
			//trueにすると、レベルの方でモデルが読み込まれない。
			return true;

		}

		if (objData.EqualObjectName(L"Light") == true) {

			//ポイントライトのオブジェクトを作る。
			PointLight* light = new PointLight;
			light->Init();
			light->SetColor(Vector3(400.0f, 200.0f, 100.0f));
			light->SetAffectPowParam(5.0f);
			light->SetRange(400.0f);
			light->SetPosition(objData.position);
			m_lightArray.push_back(light);

			SpotLight* sptLight = new SpotLight;
			sptLight->Init();
			sptLight->SetRange(750.0f);
			// 色
			sptLight->SetColor(Vector3(5.0f, 1.0f, 1.0f));
			sptLight->SetColor2(Vector3(100.0f, 60.0f, 20.0f));
			sptLight->SetColor3(Vector3(8.0f, 1.5f, 1.5f));
			// 角度の減衰率
			sptLight->SetAngleAffectPowParam(1.0f);
			sptLight->SetAngleAffectPowParam2(1.0f);
			sptLight->SetAngleAffectPowParam3(1.5f);
			// 範囲の減衰率
			sptLight->SetRangeAffectPowParam(5.0f);
			sptLight->SetRangeAffectPowParam2(100.0f);
			sptLight->SetRangeAffectPowParam3(20.0f);
			// 角度
			sptLight->SetAngle(Math::DegToRad(45.0f));
			sptLight->SetAngle2(Math::DegToRad(45.0f));
			sptLight->SetAngle3(Math::DegToRad(45.0f));

			sptLight->SetPosition(objData.position);
			sptLight->SetDirection(0.0f, -1.0f, 0.0f);


			m_sptLightArray.push_back(sptLight);

			VolumeSpotLight* vlspLig = new VolumeSpotLight;
			vlspLig->Init(*sptLight);
			m_volumeSptLightArray.push_back(vlspLig);


			//falseにすると、レベルの方でモデルが読み込まれて配置される。
			return true;
		}

		if (objData.EqualObjectName(L"player") == true) {

			//プレイヤーのオブジェクトを作る。
			m_player = NewGO<Player>(0, "player");
			m_player->SetPosition(objData.position);
			m_player->SetRestartPosition(objData.position);
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
		if (objData.ForwardMatchName(L"item") == true) {

			/*itemNum = objData.number;
			for (int i = 1; i < 11; i++)
			{
				int ran = 0;
				ran = rand() % 2;
				if (i == itemNum)
				{
					if (ran == 0)
					{
						m_dash = NewGO<ItemDash>(0, "dash");
						m_dash->SetPosition(objData.position);
					}
					else
					{
						m_magic = NewGO<ItemMagic>(0, "magic");
						m_magic->SetPosition(objData.position);
					}
				}
			}*/
			//falseにすると、レベルの方でモデルが読み込まれて配置される。
			
			m_dash = NewGO<ItemDash>(0, "dash");
			m_dashPos = objData.position;
			m_dashPos.y = objData.position.y + 30.0f;
			//m_dash->SetPosition(objData.position);
			m_dash->SetPosition(m_dashPos);
			
			return true;
		}
		if (objData.EqualObjectName(L"gem") == true) {
			//プレイヤーのオブジェクトを作る。
			m_gem = NewGO<Gem>(0, "gem");
			m_gemPos = objData.position;
			m_gemPos.y = objData.position.y + 30.0f;
			m_gem->SetPosition(m_gemPos);
			//falseにすると、レベルの方でモデルが読み込まれて配置される。
			return true;
		}
		return true;
		});

	//ゲームカメラのオブジェクトを作る。
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");

	//m_map = NewGO<Map>(0, "map");

	return true;
}

void Game::Update()
{
	for (auto ptLig : m_lightArray) {
		ptLig->Update();
	}
	for (auto sptLig : m_sptLightArray) {
		sptLig->Update();
	}
	for (auto vlLig : m_volumeSptLightArray) {
		vlLig->Update();
	}
	ProcessState();

	AlphaValue();
	m_Prologue1.Update();
	m_Prologue2.Update();
	m_Prologue3.Update();
	m_Prologue4.Update();
	m_Prologue5.Update();
}



void Game::ProcessState()
{
	if (m_gameState == enGameState_Start)
	{
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_gameState = enGameState_Game;
			m_cameraFlag = true;
		}
	}
	else if (m_gameState == enGameState_Prologue)
	{
		Prologue();
		/*if (m_pro_page == 6) {
			m_pro_page = 0;
			m_gameState = enGameState_Game;
			m_cameraFlag = true;
		}*/
	}
	else if (m_gameState == enGameState_Game)
	{
		if (m_player->Clear() == true)
		{
			m_gameState = enGameState_Event;
		}
	}
	else if (m_gameState == enGameState_Event)
	{
		m_gameState = enGameState_Score;
	}
	else if (m_gameState == enGameState_Score)
	{
		m_gameState = enGameState_Start;
	}
	/*if (m_fadeTimer > 0.0f)
	{
		m_fadeTimer -= GameTime().GetFrameDeltaTime();
		if(m_fadeTimer <= 0.0f)
		{
			m_fadeTimer = 0.0f;
			m_fade->StartFadeIn();
		}
	}*/
}

void Game::Prologue()
{
	switch (m_pro_page)
	{
	case 1:
		PageUp();
	case 2:
		PageUp();
	case 3:
		PageUp();
	case 4:
		PageUp();
	case 5:
		PageUp();
	case 6:
		m_cameraFlag = true;
		m_gameState = enGameState_Game;
		m_pro_page = 0;
		break;
	default:
		break;
	}
}


void Game::Render(RenderContext& rc)
{
	if (m_gameState != enGameState_Prologue)
	{
		return;
	}
	else
	{
		if (m_alpha > 0.0f) {
			switch (m_pro_page)
			{
			case 1:
				m_Prologue1.SetMulColor({ 1.0f, 1.0f, 1.0f, m_alpha });
				m_Prologue1.Draw(rc);
			case 2:
				m_Prologue2.SetMulColor({ 1.0f, 1.0f, 1.0f, m_alpha });
				m_Prologue2.Draw(rc);
			case 3:
				m_Prologue3.SetMulColor({ 1.0f, 1.0f, 1.0f, m_alpha });
				m_Prologue3.Draw(rc);
			case 4:
				m_Prologue4.SetMulColor({ 1.0f, 1.0f, 1.0f, m_alpha });
				m_Prologue4.Draw(rc);
			case 5:
				m_Prologue5.SetMulColor({ 1.0f, 1.0f, 1.0f, m_alpha });
				m_Prologue5.Draw(rc);
			}
		}
	}
}