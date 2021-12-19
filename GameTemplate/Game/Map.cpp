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
	//マップの黒い画像。
	m_mapSprite.Init("Assets/sprite/map.DDS", 512, 512);
	m_mapSprite.SetPosition(MAP_CENTER_POSITION);

	//マップ中心の赤い画像。
	m_playerSprite.Init("Assets/sprite/player.dds", 50, 50);
	m_playerSprite.SetPosition(MAP_CENTER_POSITION);

	//エネミーを表す、黄色い画像。
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

	//マップに表示する範囲にジェムがあったら。
	if (WorldPositionConvertToMapPosition(playerPos, gemsPos, mapPos))
	{
		//マップに表示するように設定する。
		m_isImage = true;
		//SpriteRenderに座標を設定。
		m_gemSprite.SetPosition(gemsPos);
	}
	//マップに表示する範囲にジェムがなかったら。
	else
	{
		//マップに表示しない
		m_isImage = false;
	}
	m_mapSprite.Update();
	m_playerSprite.Update();
	m_gemSprite.Update();
}

const bool Map::WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 worldPosition, Vector3& mapPosition)
{
	//Y座標0
	worldCenterPosition.y = 0.0f;
	worldPosition.y = 0.0f;
	Vector3 diff = worldPosition - worldCenterPosition;

	//マップの中心とするオブジェクトとの距離が一定以上はなれていたら
	if (diff.LengthSq() >= LIMITED_RANGE_IMAGE * LIMITED_RANGE_IMAGE)
	{
		//表示しないようにする
		return false;
	}

	//ベクトルの長さを取得
	float length = diff.Length();

	//カメラの前方向ベクトルから
	//クォータニオンを生成。
	Vector3 forword = g_camera3D->GetForward();
	Quaternion rot;
	rot.SetRotationY(atan2(-forword.x, forword.z));

	//ベクトルにカメラの回転を適用。
	rot.Apply(diff);

	//ベクトルを正規化
	diff.Normalize();

	//マップの大きさ/距離制限
	//ベクトルをマップ座標系に変換
	diff *= length * MAP_RADIUS / LIMITED_RANGE_IMAGE;

	//マップの中央座標系に上記ベクトルを加算する。
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