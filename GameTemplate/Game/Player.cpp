#include "stdafx.h"
#include "Player.h"
#include "Game.h"
#include "Clock.h"
#include "ItemDash.h"
#include "Enemy.h"
#include "Fade.h"
#include "Score.h"

//CollisionObjectを使用するために、ファイルをインクルードする。
#include "collision/CollisionObject.h"
//EffectEmitterを使用するために、ファイルをインクルードする。
#include "graphics/effect/EffectEmitter.h"

namespace
{
	const float WIDTH = 1920.f;
	const float HIGH = 1080.f;
	const float WINGS_SPRITE = 170.0f;
	const float HEART_SPRITE = 130.0f;
}

bool Player::Start()
{
	//アニメーションのロード
	animationClips[enAnimationClip_Idle].Load("Assets/animData/jackie/idle2.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Run].Load("Assets/animData/jackie/run.tka");
	animationClips[enAnimationClip_Run].SetLoopFlag(true);
	animationClips[enAnimationClip_FastRun].Load("Assets/animData/jackie/fastrun.tka");
	animationClips[enAnimationClip_FastRun].SetLoopFlag(true);
	animationClips[enAnimationClip_Catch].Load("Assets/animData/jackie/catch.tka");
	animationClips[enAnimationClip_Catch].SetLoopFlag(true);
	animationClips[enAnimationClip_Down].Load("Assets/animData/jackie/down.tka");
	animationClips[enAnimationClip_Down].SetLoopFlag(false);
	animationClips[enAnimationClip_StandUp].Load("Assets/animData/jackie/standup.tka");
	animationClips[enAnimationClip_StandUp].SetLoopFlag(false);

	//Jackieモデルを読み込む。
	m_modelRender.Init("Assets/modelData/human/jackie.tkm", animationClips, enAnimationClip_Num, enModelUpAxisZ, true);
	m_modelRender.SetScale({2.0f,2.0f,2.0f});
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetShadowCasterFlag(true);
	m_modelRender.Update();
	//キャラコンを初期化する。
	m_characterController.Init(20.0f, 90.0f, m_position);

	m_life1Render.Init("Assets/sprite/life/life1.dds", HEART_SPRITE, HEART_SPRITE);
	m_life1Render.SetPosition(Vector3(0.0f, 10.0f, 0.0f));
	m_life2Render.Init("Assets/sprite/life/life2.dds", HEART_SPRITE, HEART_SPRITE);
	m_life1Render.SetPosition(Vector3(0.0f, 10.0f, 0.0f));
	m_life3Render.Init("Assets/sprite/life/life3.dds", HEART_SPRITE, HEART_SPRITE);
	m_life1Render.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_wingsRender.Init("Assets/sprite/UI/wings.dds", WINGS_SPRITE, WINGS_SPRITE);
	m_wingsRender.SetPosition(Vector3(-830.0f, 350.0f, 0.0f));

	//各クラスのFindGO
	m_dash = FindGO<ItemDash>("dash");
	m_game = FindGO<Game>("game");
	m_enemys = FindGOs<Enemy>("enemy");

	m_fade = FindGO<Fade>("fade");
	m_isGamenow = true;
	return true;
}

Player::Player()
{
	
}

Player::~Player()
{
	m_score = FindGO<Score>("score");
	m_score->SetUseItem(m_useItem);
	m_score->SetLeftGem(m_gemCount);
}

//更新処理。
void Player::Update()
{
	//移動処理。
	Move();
	//回転処理。
	Rotation();
	//ステートの遷移処理。
	ManageState();
	//つかみ判定
	Catch();
	//クリア判定
	Clear();
	//アニメーションの再生。
	PlayAnimation();
	//文字表示
	Font();
	//モデルの更新。
	m_modelRender.Update();
	m_wingsRender.Update();
	m_life1Render.Update();
	m_life2Render.Update();
	m_life3Render.Update();
}

void Player::Clear() {
	if (m_gemCount == 0 && m_isGamenow == true)
	{
		NewGO<Clock>(0, "clock");
	}
}

//文字表示
void Player::Font()
{
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"x %d", m_dashCount);
	//表示するテキストを設定。
	m_leftdash_Render.SetText(wcsbuf);
	//フォントの位置を設定。
	m_leftdash_Render.SetPosition(Vector3(-760.0f, 400.0f, 0.0f));
	//フォントの大きさを設定。
	m_leftdash_Render.SetScale(2.0f);
	swprintf_s(wcsbuf, 256, L"Aで立ち上がる");
	//表示するテキストを設定。
	m_downCommentRender.SetText(wcsbuf);
	//フォントの位置を設定。
	m_downCommentRender.SetPosition(Vector3(0.0f, -200.0f, 0.0f));
	//フォントの大きさを設定。
	m_downCommentRender.SetScale(1.5f);
	swprintf_s(wcsbuf, 256, L"夢のかけら のこり%d", m_gemCount);
	//表示するテキストを設定。
	m_gemRender.SetText(wcsbuf);
	//フォントの位置を設定。
	m_gemRender.SetPosition(Vector3(100.0f, 500.0f, 0.0f));
	//フォントの大きさを設定。
	m_gemRender.SetScale(1.5f);
	
}

//つかみ判定
void Player::Catch()
{
	if (m_notCatchTimer > 0.0f) 
	{
		m_notCatchTimer -= g_gameTime->GetFrameDeltaTime();
		
	}
	else
	{
		m_notCatchTimer = 0.0f;
	}
}

//移動処理
void Player::Move()
{
	if (IsEnableMove() == false)
	{
		return;
	}
	//xzの移動速度を0.0fにする。
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//左スティックの入力量を取得。
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//カメラの前方向と右方向のベクトルを持ってくる。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	//y方向には移動させない。
	forward.y = 0.0f;
	right.y = 0.0f;

	//移動速度にスティックの入力量を加算する。
	m_moveSpeed += right * stickL.x * 350.0f;
	m_moveSpeed += forward * stickL.y * 350.0f;

	//ダッシュ状態なら速度を1.6倍にする。
	if (m_playerState == enPlayerState_FastRun) 
	{
		m_moveSpeed.x *= 1.3f;
		m_moveSpeed.z *= 1.3f;
	}


	////////////////////////////////


	//地面に付いていたら。
	if (m_characterController.IsOnGround())
	{
		//重力を無くす。
		m_moveSpeed.y = 0.0f;
	}
	//地面に付いていなかったら。
	else
	{
		//重力を発生させる。
		m_moveSpeed.y -= 2.5f;
	}

	//キャラクターコントローラーを使って座標を移動させる。
	m_position = m_characterController.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	//絵描きさんに座標を教える。
	m_modelRender.SetPosition(m_position);
}

//回転
void Player::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
		//このフレームではキャラは移動していないので旋回する必要はない。
		return;
	}
	//atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数。
	//m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる。
	//atan2を使用して、角度を求めている。
	//これが回転角度になる。
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atanが返してくる角度はラジアン単位なので
	//SetRotationDegではなくSetRotationを使用する。
	m_rotation.SetRotationY(-angle);

	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);

	//プレイヤーの正面ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

//アイドルステート
void Player::IdleState()
{
	ProcessState();
}

//移動ステート
void Player::RunState()
{
	ProcessState();
}

//速度アップステート
void Player::FastRunState()
{
	if (m_timer > 0.0f) {
		m_timer -= g_gameTime->GetFrameDeltaTime();
	}
	else 
	{
		m_timer = 0.0f;
	}
	ProcessState();
}

//キャッチステート
void Player::CatchState()
{
	//ダウンしたら。
	if (m_death == true)
	{
		m_downCount -= 1;
		m_playerState = enPlayerState_Down;
	}

}

//ダウンステート
void Player::DownState()
{
	//アニメーションが再生中なら。
	if (m_modelRender.IsPlayingAnimation() == true)
	{
		//処理しない。
		//早めのリターン。
		//早期リターン。
		return;
	}

	//ダウンカウントが0なら。
	if (m_downCount ==0)
	{
		//ゲームオーバー。
		m_game->SetGameOverFlag();
		m_isGamenow = false;
		return;
	}

	m_downScreen = true;
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_death = false;
		m_playerState = enPlayerState_StandUp;
	}
}

void Player::StandUpState()
{
	if (m_modelRender.IsPlayingAnimation() == true)
	{
		//処理しない。
		return;
	}
	m_playerState = enPlayerState_Idle;
}

//ステート遷移
void Player::ProcessState()
{
	if (m_playerState != enPlayerState_Catch) {
		//Bボタンが押されたら。
		if (g_pad[0]->IsTrigger(enButtonB) && m_fastRun == false && m_dashCount > 0)
		{
			//カウント-1
			m_dashCount -= 1;
			m_useItem += 1;
			//ダッシュタイムを4秒に設定。
			m_timer = 4.0f;
			m_fastRun = true;
			return;
		}
	}
	for (int i = 0; i < 2; i++) {
		if (m_enemys[i]->GetCatchState() == true)
		{
			m_playerState = enPlayerState_Catch;
			return;
		}
	}

	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		if (m_timer > 0.0f) {
			m_playerState = enPlayerState_FastRun;
		}
		else {
			//ステートを走りにする。
			m_playerState = enPlayerState_Run;
			m_fastRun = false;
		}
		return;
	}
	//ステートを待機にする。
	m_playerState = enPlayerState_Idle;
	
	
}

//ステート管理。
void Player::ManageState()
{
	switch (m_playerState)
	{
		//待機ステートの時。
	case enPlayerState_Idle:
		//待機ステートのステート遷移処理。
		IdleState();
		break;
		//Runステートの時。
	case enPlayerState_Run:
		//Runステートのステート遷移処理。
		RunState();
		break;
		//FastRunステートの時。
	case enPlayerState_FastRun:
		//FastRunステートのステート遷移処理。
		FastRunState();
		break;
		//ダウンステートの時。
	case enPlayerState_Down:
		//ダウンステートのステート遷移処理。
		DownState();
		break;
	case enPlayerState_Catch:
		CatchState();
		break;
		//クリアステートの時。
	case enPlayerState_StandUp:
		//クリアステートのステート遷移処理。
		StandUpState();
		break;
	default:
		break;
	}
}

//アニメーションの再生。
void Player::PlayAnimation()
{
	//switch文。
	switch (m_playerState) {
	//プレイヤーステートがIdleだったら。
	case enPlayerState_Idle:
		//Idle
		m_modelRender.PlayAnimation(enAnimationClip_Idle,0.3f);
		break;
	//プレイヤーステートがRunだったら。
	case enPlayerState_Run:
		//Run
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.3f);
		break;
	//プレイヤーステートがFastRunだったら。
	case enPlayerState_FastRun:
		//FastRun
		m_modelRender.PlayAnimation(enAnimationClip_FastRun, 0.3f);
		break;
	//プレイヤーステートがCatchだったら。
	case enPlayerState_Catch:
		//Catch
		m_modelRender.PlayAnimation(enAnimationClip_Catch, 0.3f);
		break;
	//プレイヤーステートがDownだったら。
	case enPlayerState_Down:
		//Down
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.3f);
		break;
	case enPlayerState_StandUp:
		//Down
		m_modelRender.PlayAnimation(enAnimationClip_StandUp, 0.3f);
		break;
	default:
		break;
	}
}

//描画処理。
void Player::Render(RenderContext& rc)
{
	//モデルを描画する。
	m_modelRender.Draw(rc);
	m_gemRender.Draw(rc);
	m_wingsRender.Draw(rc);
	m_life1Render.Draw(rc);
	m_life2Render.Draw(rc);
	m_life3Render.Draw(rc);
	//if (m_downCount >= 3)
	//{
	//	m_life3Render.Draw(rc);
	//}
	//if (m_downCount >= 2)
	//{
	//	m_life2Render.Draw(rc);
	//}
	//
	m_leftdash_Render.Draw(rc);
	if (m_death == true) {
		m_downCommentRender.Draw(rc);
	}
}
