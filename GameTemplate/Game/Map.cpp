#include "stdafx.h"
#include "Map.h"
#include "Player.h"
#include "Gem.h"

namespace
{
	Vector3 MAP_CENTER_POSITION = Vector3(704.0f, 284.0f, 0.0f);

	float MAP_RADIUS = 240.0f;
	float LIMITED_RANGE_IMAGE = 600.0f;
}

bool Map::Start() 
{
	//�}�b�v�̍����摜�B
	m_mapSprite.Init("Assets/sprite/map.DDS", 512, 512);
	m_mapSprite.SetPosition(MAP_CENTER_POSITION);

	//�}�b�v���S�̐Ԃ��摜�B
	m_playerSprite.Init("Assets/sprite/player.dds", 50, 50);
	m_playerSprite.SetPosition(MAP_CENTER_POSITION);

	//�G�l�~�[��\���A���F���摜�B
	m_gemSprite.Init("Assets/sprite/enemy.dds", 35, 35);

	//m_gems = FindGOs<Gem>("gem");
	
	m_player = FindGO<Player>("player");

	return true;
}

void Map::Update()
{
	
	

	Vector3 playerPos = m_player->GetPosition();

	const auto& gem = FindGOs<Gem>("gem");
	const int gemSize = gem.size();

	for (int i = 0; i < gemSize; i++) 
	{
		gemsPos[i] = gem->GetPosition();
	}

	Vector3 mapPos;

	//�}�b�v�ɕ\������͈͂ɃW�F������������B
	if (WorldPositionConvertToMapPosition(playerPos, gemsPos, mapPos))
	{
		//�}�b�v�ɕ\������悤�ɐݒ肷��B
		m_isImage = true;
		//SpriteRender�ɍ��W��ݒ�B
		m_gemSprite.SetPosition(gemsPos);
	}
	//�}�b�v�ɕ\������͈͂ɃW�F�����Ȃ�������B
	else
	{
		//�}�b�v�ɕ\�����Ȃ�
		m_isImage = false;
	}
	m_mapSprite.Update();
	m_playerSprite.Update();
	m_gemSprite.Update();
}

const bool Map::WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 worldPosition, Vector3& mapPosition)
{
	//Y���W0
	worldCenterPosition.y = 0.0f;
	worldPosition.y = 0.0f;
	Vector3 diff = worldPosition - worldCenterPosition;

	//�}�b�v�̒��S�Ƃ���I�u�W�F�N�g�Ƃ̋��������ȏ�͂Ȃ�Ă�����
	if (diff.LengthSq() >= LIMITED_RANGE_IMAGE * LIMITED_RANGE_IMAGE)
	{
		//�\�����Ȃ��悤�ɂ���
		return false;
	}

	//�x�N�g���̒������擾
	float length = diff.Length();

	//�J�����̑O�����x�N�g������
	//�N�H�[�^�j�I���𐶐��B
	Vector3 forword = g_camera3D->GetForward();
	Quaternion rot;
	rot.SetRotationY(atan2(-forword.x, forword.z));

	//�x�N�g���ɃJ�����̉�]��K�p�B
	rot.Apply(diff);

	//�x�N�g���𐳋K��
	diff.Normalize();

	//�}�b�v�̑傫��/��������
	//�x�N�g�����}�b�v���W�n�ɕϊ�
	diff *= length * MAP_RADIUS / LIMITED_RANGE_IMAGE;

	//�}�b�v�̒������W�n�ɏ�L�x�N�g�������Z����B
	mapPosition = Vector3(MAP_CENTER_POSITION.x + diff.x, MAP_CENTER_POSITION.y + diff.z, 0.0f);
	return true;
}

void Map::Render(RenderContext& rc)
{
	m_mapSprite.Draw(rc);
	m_playerSprite.Draw(rc);
	if(m_isImage == false)
	{
		return;
	}
	m_gemSprite.Draw(rc);

	/*const auto& gems = FindGOs<Gem>("gem");
	const int gemSize = gems.size();

	for (int i = 0; i < gemSize; i++)
	{
		gems[i]->GemMap(rc);
	}*/
}