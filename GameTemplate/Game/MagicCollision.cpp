#include "stdafx.h"
#include "MagicCollision.h"

#include "Player.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//CollisionObjectを使用したいので、ファイルをインクルードする。
#include "collision/CollisionObject.h"

//EffectEmitterを使用したいので、ファイルをインクルードする。
#include "graphics/effect/EffectEmitter.h"

MagicCollision::MagicCollision()
{

}

MagicCollision::~MagicCollision()
{
	//コリジョンオブジェクトを削除する。
	DeleteGO(m_collisionObject);
	////エフェクトの再生を停止する。
	//m_effectEmitter->Stop();
	////エフェクトを削除する。
	//DeleteGO(m_effectEmitter);
}

bool MagicCollision::Start()
{

	m_player = FindGO<Player>("player");

	Vector3 collisionObject = m_position;

	m_collisionObject = NewGO< CollisionObject>(0);
	//球状のコリジョン作成
	m_collisionObject->CreateSphere(
		collisionObject,
		Quaternion::Identity,
		30.0f
	);
	//コリジョンの名前を設定
	m_collisionObject->SetName("player_magic");
	//コリジョンオブジェクトが自動で削除されないようにする。
	m_collisionObject->SetIsEnableAutoDelete(false);

	return true;
}

void MagicCollision::Move()
{
	//移動速度を計算。
	m_position += m_moveSpeed * 15.0f;
}

void MagicCollision::Update()
{
	Move();
	m_collisionObject->SetPosition(m_position);

	//タイマー
	m_timer += g_gameTime->GetFrameDeltaTime();


	//タイマーが0.7f以上だったら。
	if (m_timer >= 0.9f)
	{
		//自身を削除する。
		DeleteGO(this);
	}
}