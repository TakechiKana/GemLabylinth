#include<random>
#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "Player.h"
#include "GameCamera.h"
#include "Item.h"
#include "ItemDash.h"
#include "Enemy.h"
#include "Gem.h"
#include "Fade.h"
#include "Title.h"
//#include "Map.h"


Game::~Game()
{
	DeleteGO(m_gameCamera);
	DeleteGO(m_player);
	DeleteGO(m_backGround);
	//DeleteGO(m_bgm);

	const auto& enemys = FindGOs<Enemy>("enemy");
	for (auto enemy : enemys)
	{
		DeleteGO(enemy);
	}

	for (auto ptLig : m_lightArray) {
		delete ptLig;
	}
}

bool Game::Start()
{

	m_player = FindGO<Player>("player");

	g_renderingEngine->SetSceneMiddleGray(0.1f);
	g_renderingEngine->SetAmbient({ 0.1f, 0.1f, 0.1f });

	

	m_levelRender.Init("Assets/modelData/stage/stage2.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"stage") == true) {

			m_backGround = NewGO<BackGround>(0);
			//�z�u���W�A�X�P�[���A��]���擾����B
			m_backGround->SetPosition(objData.position);
			//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
			return true;

		}

		if (objData.EqualObjectName(L"Light") == true) {

			//�|�C���g���C�g�̃I�u�W�F�N�g�����B
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
			// �F
			sptLight->SetColor(Vector3(5.0f, 1.0f, 1.0f));
			sptLight->SetColor2(Vector3(100.0f, 60.0f, 20.0f));
			sptLight->SetColor3(Vector3(8.0f, 1.5f, 1.5f));
			// �p�x�̌�����
			sptLight->SetAngleAffectPowParam(1.0f);
			sptLight->SetAngleAffectPowParam2(1.0f);
			sptLight->SetAngleAffectPowParam3(1.5f);
			// �͈͂̌�����
			sptLight->SetRangeAffectPowParam(5.0f);
			sptLight->SetRangeAffectPowParam2(100.0f);
			sptLight->SetRangeAffectPowParam3(20.0f);
			// �p�x
			sptLight->SetAngle(Math::DegToRad(45.0f));
			sptLight->SetAngle2(Math::DegToRad(45.0f));
			sptLight->SetAngle3(Math::DegToRad(45.0f));

			sptLight->SetPosition(objData.position);
			sptLight->SetDirection(0.0f, -1.0f, 0.0f);


			m_sptLightArray.push_back(sptLight);

			VolumeSpotLight* vlspLig = new VolumeSpotLight;
			vlspLig->Init(*sptLight);
			m_volumeSptLightArray.push_back(vlspLig);


			//false�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ĕz�u�����B
			return true;
		}

		if (objData.EqualObjectName(L"player") == true) {

			//�v���C���[�̃I�u�W�F�N�g�����B
			m_player = NewGO<Player>(0, "player");
			m_player->SetPosition(objData.position);
			m_player->SetRestartPosition(objData.position);
			m_player->SetRotation(objData.rotation);


			//false�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ĕz�u�����B
			return true;
		}

		if (objData.ForwardMatchName(L"enemy") == true) 
		{
			//�v���C���[�̃I�u�W�F�N�g�����B
			m_enemy = NewGO<Enemy>(0, "enemy");
			m_enemy->SetPosition(objData.position);

			//false�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ĕz�u�����B
			return true;
		}
		if (objData.ForwardMatchName(L"item") == true) 
		{
			m_dash = NewGO<ItemDash>(0, "dash");
			m_dashPos = objData.position;
			m_dashPos.y = objData.position.y + 30.0f;
			//m_dash->SetPosition(objData.position);
			m_dash->SetPosition(m_dashPos);
			
			return true;
		}
		if (objData.EqualObjectName(L"gem") == true) {
			//�v���C���[�̃I�u�W�F�N�g�����B
			m_gem = NewGO<Gem>(0, "gem");
			m_gemPos = objData.position;
			m_gemPos.y = objData.position.y + 30.0f;
			m_gem->SetPosition(m_gemPos);
			//false�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ĕz�u�����B
			return true;
		}
		return true;
	});

	//�Q�[���J�����̃I�u�W�F�N�g�����B
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");

	//m_map = NewGO<Map>(0, "map");

	//�t�F�[�h���I������B
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

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
}

void Game::Render(RenderContext& rc)
{

}