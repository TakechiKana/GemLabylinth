#include "stdafx.h"
#include "Enemy.h"

#include "Game.h"
#include "Player.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

#include <time.h>
#include <stdlib.h>

//CollisionObjectを使用したいため、ファイルをインクルードする。
#include "collision/CollisionObject.h"


Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

bool Enemy::Start()
{
	//アニメーションを読み込む。
	//m_animationClips[enAnimationClip_Idle].Load("Assets/animData/enemy/idle.tka");
	//m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	//m_animationClips[enAnimationClip_Walk].Load("Assets/animData/enemy/walk.tka");
	//m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	//m_animationClips[enAnimationClip_Run].Load("Assets/animData/enemy/run.tka");
	//m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	//m_animationClips[enAnimationClip_Attack].Load("Assets/animData/enemy/attack.tka");
	//m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	//m_animationClips[enAnimationClip_MagicAttack].Load("Assets/animData/enemy/magicattack.tka");
	//m_animationClips[enAnimationClip_MagicAttack].SetLoopFlag(false);
	//m_animationClips[enAnimationClip_Damage].Load("Assets/animData/enemy/receivedamage.tka");
	//m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	//m_animationClips[enAnimationClip_Down].Load("Assets/animData/enemy/down.tka");
	//m_animationClips[enAnimationClip_Down].SetLoopFlag(false);

	//モデルを読み込む。
	m_modelRender.Init("Assets/modelData/human/jackie.tkm"/*, m_animationClips, enAnimationClip_Num*/);
	//ナビメッシュ
	m_nvmMesh.Init("Assets/modelData/stage/stage_mesh.tkn");

	//座標を設定する。
	Vector3 position = m_position;
	position.y += 2.5f;

	m_modelRender.SetScale({2.0f,2.0f,2.0f});

	m_modelRender.SetPosition(position);
	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);
	//大きさを設定する。
	//m_modelRender.SetScale(m_scale);

	//キャラクターコントローラーを初期化。
	m_charaCon.Init(
		20.0f,			//半径。
		100.0f,			//高さ。
		m_position		//座標。
	);


	//音を読み込む。
	//g_soundEngine->ResistWaveFileBank(0, "Assets/sound/magic.wav");
	//g_soundEngine->ResistWaveFileBank(3, "Assets/sound/slash.wav");
	//g_soundEngine->ResistWaveFileBank(4, "Assets/sound/hit.wav");

	//playerクラスの検索
	m_player = FindGO<Player>("player");
	//乱数を初期化。
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	m_targetPosition = m_player->GetPosition();
	return true;
}

void Enemy::Update()
{
	//移動処理
	Move();
	//追跡処理。
	//Chase();
	//回転処理。
	Rotation();
	//当たり判定。
	Collision();
	//攻撃処理。
	Attack();
	//アニメーションの再生。
	PlayAnimation();
	//ステートの遷移処理。
	ManageState();

	//モデルの更新。
	m_modelRender.Update();
	m_targetPosition = m_player->GetPosition();
}

void Enemy::Move()
{
	bool isEnd;
	if (g_pad[0]->IsTrigger(enButtonUp)) {
		// パス検索
		m_pathFiding.Execute(
			m_path,							// 構築されたパスの格納先
			m_nvmMesh,						// ナビメッシュ
			m_position,						// 開始座標
			m_targetPosition,				// 移動目標座標
			PhysicsWorld::GetInstance(),	// 物理エンジン	
			10.0f,							// AIエージェントの半径
			200.0f							// AIエージェントの高さ。
		);
	}
	// パス上を移動する。
	m_position = m_path.Move(
		m_position,
		10.0f,
		isEnd
	);



	//座標を設定する。
	m_modelRender.SetPosition(m_position);
	m_charaCon.SetPosition(m_position);
	Vector3 pos = Vector3::Zero;
	m_charaCon.Execute(pos, 1.0f);
}

void Enemy::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
		//このフレームではキャラは移動していないので旋回する必要はない。
		return;
	}
	//これが回転角度になる。
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atanが返してくる角度はラジアン単位なので
	//SetRotationDegではなくSetRotationを使用する。
	m_rotation.SetRotationY(-angle);

	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);

	//プレイヤーの前ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Enemy::Chase()
{
	//追跡ステートでないなら、追跡処理はしない。
	if (m_enemyState != enEnemyState_Chase)
	{
		//return;
	}
	//エネミーを移動させる。
	if (m_charaCon.IsOnGround()) {
		//地面についた。
		//重力を0にする。
		m_moveSpeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	////ちょっとだけモデルの座標を挙げる。
	//modelPosition.y += 2.5f;
	//座標を設定する。
	//m_modelRender.SetPosition(modelPosition);
}

void Enemy::Collision()
{
	//被ダメージ、あるいはダウンステートの時は。
	//当たり判定処理はしない。
	if (m_enemyState == enEnemyState_ReceiveDamage ||
		m_enemyState == enEnemyState_Down)
	{
		return;
	}

	//プレイヤーの攻撃用のコリジョンを取得する。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//コリジョンの配列をfor文で回す。
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_charaCon))
		{
			//HPを1減らす。
			m_hp -= 1;

			//もしHPが0より上なら。
			if (m_hp > 0)
			{
				//被ダメージステートに遷移する。
				m_enemyState = enEnemyState_ReceiveDamage;
			}
			//HPが0なら。
			else if (m_hp == 0)
			{
				//ダウンステートに遷移する。
				m_enemyState = enEnemyState_Down;
			}

			//効果音を再生する。
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(4);
			se->Play(false);
			se->SetVolume(0.4f);
			return;
		}
	}

	//プレイヤーのファイヤーボール用のコリジョンを取得する。
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_fireball");
	//for文で配列を回す。
	for (auto collision : collisions2)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_charaCon))
		{
			//HPを1減らす。
			m_hp -= 1;
			//HPが0になったら。
			if (m_hp == 0)
			{
				//ダウンステートに遷移する。
				m_enemyState = enEnemyState_Down;
			}
			else {
				//被ダメージステートに遷移する。
				m_enemyState = enEnemyState_ReceiveDamage;
			}
			//効果音を再生する。
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(4);
			se->Play(false);
			se->SetVolume(0.6f);
			return;
		}
	}
}

void Enemy::Attack()
{
	//攻撃ステートでないなら処理をしない。
	if (m_enemyState != enEnemyState_Punch)
	{
		return;
	}

	//攻撃中であれば。
	if (m_isUnderAttack == true)
	{
		//攻撃用のコリジョンを作成する。
		MakePunchCollision();
	}
}

const bool Enemy::SearchPlayer() const
{
	//エネミーからプレイヤーに向かうベクトルを求める。
	Vector3 diff = m_player->GetPosition() - m_position;

	//プレイヤーにある程度近かったら。
	if (diff.Length() <= 1500.0f)
	{
		return true;
	}
	//プレイヤーを見つけられなかった。
	return false;
}

void Enemy::MakePunchCollision()
{
	//コリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);

	Vector3 collisionPosition = m_position;
	//座標をプレイヤーの少し前に設定する。
	collisionPosition += m_forward * 50.0f;
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateSphere(collisionPosition,               //座標。
		Quaternion::Identity,                                      //回転。
		20.0f												       //大きさ。
	);
	collisionObject->SetName("enemy_punch");

	//「Punch」ボーンのワールド行列を取得する。
	Matrix matrix = m_modelRender.GetBone(m_PunchBoneId)->GetWorldMatrix();
	//「Punch」ボーンのワールド行列をコリジョンに適用する。
	collisionObject->SetWorldMatrix(matrix);
}

//void Enemy::MakeMagic()
//{
//
//}

void Enemy::ProcessState()
{
	//各タイマーを初期化
	//待機時間と追跡時間を制限するため。
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//エネミーからプレイヤーに向かうベクトルを計算する。
	Vector3 diff = m_player->GetPosition() - m_position;

	//プレイヤーを見つけたら。
	if (SearchPlayer() == true)
	{
		//ベクトルを正規化する。
		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff * 250.0f;
		//通常攻撃できない距離なら
		if (IsCanPunch() == false)
		{	
			return;
		}
	}
	//プレイヤーを見つけられなければ。
	else
	{
		//待機ステートに遷移する。
		m_enemyState = enEnemyState_Idle;
		return;
	}
}

void Enemy::IdleState()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//待機時間がある程度経過したら。
	if (m_idleTimer >= 0.9f)
	{
		//他のステートへ遷移する。
		ProcessState();
	}

}

void Enemy::WalkState()
{
	//他のステートに遷移する。
	ProcessState();
}

void Enemy::RunState()
{
	//他のステートに遷移する。
	ProcessState();
}

void Enemy::ChaseState()
{
	//攻撃できる距離なら。
	if (IsCanPunch() == true)
	{
		//他のステートに遷移する。
		ProcessState();
		return;
	}
	m_chaseTimer += g_gameTime->GetFrameDeltaTime();
	//追跡時間がある程度経過したら。
	if (m_chaseTimer >= 0.8f)
	{
		//他のステートに遷移する。
		ProcessState();
	}
}

void Enemy::PunchState()
{
	//攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する。
		ProcessState();
	}
}

//void Enemy::ProcessMagicAttackStateTransition()
//{
//	//魔法攻撃アニメーションの再生が終わったら。
//	if (m_modelRender.IsPlayingAnimation() == false)
//	{
//		//他のステートに遷移する。
//		ProcessState();
//	}
//}

void Enemy::DamageState()
{
	//被ダメージアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//攻撃されたら距離関係無しに、取り敢えず追跡させる。
		m_enemyState = enEnemyState_Chase;
		Vector3 diff = m_player->GetPosition() - m_position;
		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff * 250.0f;
	}
}

void Enemy::DownState()
{
	//ダウンアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		Game* game = FindGO<Game>("game");
		//倒されたエネミーの数を+1する。
		//自身を削除する。
		DeleteGO(this);
	}
}

void Enemy::ManageState()
{
	switch (m_enemyState)
	{
		//待機ステートの時。
	case enEnemyState_Idle:
		//待機ステートのステート遷移処理。
		IdleState();
		break;
		//追跡ステートの時。
	case enEnemyState_Chase:
		//追跡ステートのステート遷移処理。
		ChaseState();
		break;
		//攻撃ステートの時。
	case enEnemyState_Punch:
		//攻撃ステートのステート遷移処理。
		PunchState();
		break;
		//魔法攻撃ステートの時。
	//case enEnemyState_MagicAttack:
	//	//魔法攻撃ステートのステート遷移処理。
	//	ProcessMagicAttackStateTransition();
	//	break;
		//被ダメージステートの時。
	case enEnemyState_ReceiveDamage:
		//被ダメージステートのステート遷移処理。
		DamageState();
		break;
		//ダウンステートの時。
	case enEnemyState_Down:
		//ダウンステートのステート遷移処理。
		DownState();
		break;
	}
}

void Enemy::PlayAnimation()
{
	//m_modelRender.SetAnimationSpeed(1.0f);
	//switch (m_enemyState)
	//{
	//	//待機ステートの時。
	//case enEnemyState_Idle:
	//	//待機アニメーションを再生。
	//	m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
	//	break;
	//	//追跡ステートの時。
	//case enEnemyState_Chase:
	//	m_modelRender.SetAnimationSpeed(1.2f);
	//	//走りアニメーションを再生。
	//	m_modelRender.PlayAnimation(enAnimationClip_Run, 0.1f);
	//	break;
	//	//攻撃ステートの時。
	//case enEnemyState_Punch:
	//	m_modelRender.SetAnimationSpeed(1.6f);
	//	//攻撃アニメーションを再生。
	//	m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
	//	break;
	//	//魔法攻撃ステートの時。
	//case enEnemyState_MagicAttack:
	//	m_modelRender.SetAnimationSpeed(1.2f);
	//	//魔法攻撃アニメーションを再生。
	//	m_modelRender.PlayAnimation(enAnimationClip_MagicAttack, 0.1f);
	//	break;
	//	//被ダメージステートの時。
	//case enEnemyState_ReceiveDamage:
	//	m_modelRender.SetAnimationSpeed(1.3f);
	//	//被ダメージアニメーションを再生。
	//	m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
	//	break;
	//	//ダウンステートの時。
	//case enEnemyState_Down:
	//	//ダウンアニメーションを再生。
	//	m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
	//	break;
	//default:
	//	break;
	//}
}

const bool Enemy::IsCanPunch() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//エネミーとプレイヤーの距離が近かったら。
	if (diff.LengthSq() <= 100.0f * 100.0f)
	{
		//攻撃できる！
		return true;
	}
	//攻撃できない。
	return false;
}

void Enemy::Render(RenderContext& rc)
{
	//モデルを描画する。
	m_modelRender.Draw(rc);
}