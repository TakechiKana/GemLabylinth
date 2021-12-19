#include "stdafx.h"
#include "GameCamera.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//注視点から視点までのベクトルを設定。
	//m_toCameraPos.Set(0.0f, 30.0f, 70.0f);
	m_toCameraPos.Set(-20.0f, 15.0f, -70.0f);

	//各クラスのインスタンスを探す。
	m_player = FindGO<Player>("player");
	m_game = FindGO<Game>("game");
	m_enemy = FindGO<Enemy>("enemy");

	//ばねカメラの初期化。
	m_springCamera.Init(
		*g_camera3D,		//ばねカメラの処理を行うカメラを指定する。
		25000.0f,			//カメラの移動速度の最大値。
		true,				//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
		5.0f				//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
	);
	return true;
}


void GameCamera::CameraPos()
{
	if (m_game->GetState() == Game::enGameState_Game && m_game->GetCameraFlag() == true)
	{
		m_toCameraPos.Set(0.0f, 30.0f, 70.0f);
		m_game->SetCameraFlag(false);
	}
}

void GameCamera::UpdatePositionAndTarget()
{
	CameraPos();
	//カメラを更新。
	//注視点を計算する。
	if (m_enemy->GetCatchState() == true)
	{
		target = m_enemy->GetPosition();
		target.y += 120.0f;
	}
	else
	{
		target = m_player->GetPosition();
		target.y += 100.0f;
	}
	//プレイヤの足元からちょっと上を注視点とする。
	//target.y += 100.0f;
	target += g_camera3D->GetForward() * 15.0f;

	if (m_game->GetState() == Game::enGameState_Game) {
		Vector3 toCameraPosOld = m_toCameraPos;
		//パッドの入力を使ってカメラを回す。
		float x = g_pad[0]->GetRStickXF() * 2.0f;
		float y = g_pad[0]->GetRStickYF() * 2.0f;
		//Y軸周りの回転
		Quaternion qRot;
		qRot.SetRotationDeg(Vector3::AxisY, 1.5f * x);
		qRot.Apply(m_toCameraPos);
		//X軸周りの回転。
		Vector3 axisX;
		axisX.Cross(Vector3::AxisY, m_toCameraPos);
		axisX.Normalize();
		qRot.SetRotationDeg(axisX, 1.5f * y);
		qRot.Apply(m_toCameraPos);
		//カメラの回転の上限をチェックする。
		//注視点から視点までのベクトルを正規化する。
		//正規化すると、ベクトルの大きさが１になる。
		//大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
		Vector3 toPosDir = m_toCameraPos;
		toPosDir.Normalize();
		if (toPosDir.y < -0.2f) {
			//カメラが上向きすぎ。
			m_toCameraPos = toCameraPosOld;
		}
		else if (toPosDir.y > 0.5f) {
			//カメラが下向きすぎ。
			m_toCameraPos = toCameraPosOld;
		}
	}

	//視点を計算する。
	Vector3 pos = target + m_toCameraPos;

	//バネカメラに注視点と視点を設定する。
	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(target);
}

void GameCamera::Update()
{
	//視点と注視点を更新する。
	UpdatePositionAndTarget();

	//カメラの更新。
	m_springCamera.Update();
}