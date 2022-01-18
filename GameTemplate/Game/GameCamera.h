#pragma once
//ばねカメラを使用したい場合は、SpringCameraをインクルードする。
#include "camera/SpringCamera.h"

//クラス宣言。
class Player;
class Game;
class Enemy;

/// <summary>
/// カメラ。
/// </summary>
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	/// <summary>
	/// 座標と視点を更新する。
	/// </summary>
	void UpdatePositionAndTarget();

private:
	Game* m_game = nullptr;
	Player* m_player = nullptr;								//プレイヤー。
	Enemy* m_enemy = nullptr;								//エネミー。
	Vector3				m_toCameraPos = Vector3::One;		//注視点から視点に向かうベクトル。
	Vector3				target;
	Vector3 m_position;
	Vector3 m_moveSpeed;
	SpringCamera		m_springCamera;						//ばねカメラ。
};
