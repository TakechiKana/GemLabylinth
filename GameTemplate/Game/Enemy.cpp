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
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/michelle/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/michelle/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Punch].Load("Assets/animData/michelle/punch.tka");
	m_animationClips[enAnimationClip_Punch].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Phose].Load("Assets/animData/michelle/catch.tka");
	m_animationClips[enAnimationClip_Phose].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/michelle/receivedamage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/michelle/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);

	//モデルを読み込む。
	m_modelRender.Init("Assets/modelData/human/Michelle.tkm", m_animationClips, enAnimationClip_Num);
	//ナビメッシュ
	m_nvmMesh.Init("Assets/modelData/stage/stage_mesh2.tkn");

	//座標を設定する。
	Vector3 position = m_position;
	position.y += 2.5f;

	m_modelRender.SetScale({2.7f,2.7f,2.7f});

	m_modelRender.SetPosition(position);
	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);
	//大きさを設定する。
	//m_modelRender.SetScale(m_scale);

	//キャラクターコントローラーを初期化。
	m_charaCon.Init(
		40.0f,			//半径。
		135.0f,			//高さ。
		m_position		//座標。
	);


	//音を読み込む。
	//g_soundEngine->ResistWaveFileBank(0, "Assets/sound/magic.wav");
	//g_soundEngine->ResistWaveFileBank(3, "Assets/sound/slash.wav");
	//g_soundEngine->ResistWaveFileBank(4, "Assets/sound/hit.wav");

	//アニメーションイベントの準備
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//どのボーンか
	m_PunchBoneId = m_modelRender.FindBoneID(L"mixamorig:RightHand");

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
	//追跡処理
	Chase();
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
	//つかまえた？
	Catch();

	//モデルの更新。
	m_modelRender.Update();

	//ターゲットポジション（プレイヤー）の更新
	m_targetPosition = m_player->GetPosition() + padding ;
}

void Enemy::Chase()
{
	if (m_enemyState != enEnemyState_Chase)
	{
		return;
	}


	bool isEnd;
	if (SearchPlayer()==true)   {
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
		4.0f,
		isEnd,
		m_velocity
	);

	//座標を設定する。
	m_modelRender.SetPosition(m_position);
	m_charaCon.SetPosition(m_position);
	Vector3 pos = Vector3::Zero;
	m_charaCon.Execute(pos, 1.0f);
}

void Enemy::Rotation()
{
	if (fabsf(m_velocity.x) < 0.001f
		&& fabsf(m_velocity.z) < 0.001f) {
		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
		//このフレームではキャラは移動していないので旋回する必要はない。
		return;
	}

	Quaternion rotation;
	rotation.SetRotationYFromDirectionXZ(m_velocity);
	m_modelRender.SetRotation(rotation);

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

	////プレイヤーの攻撃用のコリジョンを取得する。
	//const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_punch");
	////コリジョンの配列をfor文で回す。
	//for (auto collision : collisions)
	//{
	//	//コリジョンとキャラコンが衝突したら。
	//	if (collision->IsHit(m_charaCon))
	//	{
	//		m_enemyState = enEnemyState_ReceiveDamage;
	//		//if (m_isUnderDamage == false) {
	//		//	//HPを1減らす。
	//		//	m_hp -= 1;										///////////////////////////////////////////////
	//		//	m_isUnderDamage = true;
	//		//}
	//		////もしHPが0より上なら。
	//		//if (m_hp > 0)
	//		//{
	//			//被ダメージステートに遷移する。
	//			//m_enemyState = enEnemyState_ReceiveDamage;
	//		//}
	//		////HPが0なら。
	//		//else if (m_hp == 0)
	//		//{
	//		//	//ダウンステートに遷移する。
	//		//	m_enemyState = enEnemyState_Down;
	//		//}

	//		////効果音を再生する。
	//		//ここ
	//
	//		return;
	//	}
	//}

	//プレイヤーのファイヤーボール用のコリジョンを取得する。
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_magic");
	//for文で配列を回す。
	for (auto collision : collisions2)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_charaCon))
		{
			m_enemyState = enEnemyState_ReceiveDamage;
			//if (m_isUnderDamage == false) {
			//	//HPを2減らす。
			//	m_hp -= 2;	
			//	m_isUnderDamage = true;
			//}
			////HPが0になったら。
			//if (m_hp == 0)
			//{
			//	//ダウンステートに遷移する。
			//	m_enemyState = enEnemyState_Down;
			//}
			//else {
			//	//被ダメージステートに遷移する。
			//	m_enemyState = enEnemyState_ReceiveDamage;
			//}
			////効果音を再生する。
			////ここ
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
	if (diff.LengthSq() <= 1000.0f*1000.0f)
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

void Enemy::ProcessState()
{
	//プレイヤーを見つけたら。
	if (SearchPlayer() == true)
	{
		//通常攻撃できる距離なら
		if (IsCanPunch() == true)
		{	
			m_enemyState = enEnemyState_Punch;
			
		}
		else 
		{
			//プレイヤーに向かって走る
			m_enemyState = enEnemyState_Chase;
		}
	}
	//プレイヤーを見つけられなければ。
	else
	{
		//待機ステートに遷移する。
		m_enemyState = enEnemyState_Idle;
	}
	if (m_isUnderDamage == true) {
		m_enemyState = enEnemyState_ReceiveDamage;
	}
}

void Enemy::IdleState()
{
	//他のステートへ遷移する。
	ProcessState();
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
	//他のステートに遷移する。
	ProcessState();
}

void Enemy::PunchState()
{
	//攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_catch = false;
		m_enemyState = enEnemyState_Phose;
	}
}

void Enemy::PhoseState()
{
	//攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		ProcessState();
	}
}

void Enemy::DamageState()
{
	//被ダメージアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する。
		
		ProcessState();
		m_isUnderDamage = false;
	}
}

void Enemy::DownState()
{
	//ダウンアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		Game* game = FindGO<Game>("game");
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
	case enEnemyState_Phose:
		//攻撃ステートのステート遷移処理。
		PhoseState();
		break;
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
	switch (m_enemyState)
	{
		//待機ステートの時。
	case enEnemyState_Idle:
		//待機アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.3f);
		break;
		//追跡ステートの時。
	case enEnemyState_Chase:
		//走りアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.3f);
		break;
		//攻撃ステートの時。
	case enEnemyState_Punch:
		//攻撃アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Punch, 0.3f);
		break;
	case enEnemyState_Phose:
		//攻撃アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Phose, 0.3f);
		break;
		//被ダメージステートの時。
	case enEnemyState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.3f);
		//被ダメージアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.3f);
		break;
		//ダウンステートの時。
	case enEnemyState_Down:
		//ダウンアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.3f);
		break;
	default:
		break;
	}
}

void Enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//キーの名前が「attack_start」の時。
	if (wcscmp(eventName, L"attack_start") == 0) {
		//攻撃中にする。
		m_isUnderAttack = true;
		//エフェクトを発生させる。
		//MakeSlashingEffect();
	}
	//キーの名前が「attack_end」の時。
	else if (wcscmp(eventName, L"attack_end") == 0) {
		//攻撃を終わる。
		m_isUnderAttack = false;
	}
}

const bool Enemy::IsCanPunch() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//エネミーとプレイヤーの距離が近かったら。
	if (diff.Length() <= 70.0f)
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