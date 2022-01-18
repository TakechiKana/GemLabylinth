#include "stdafx.h"
#include "Player.h"
#include "Game.h"
//#include "ItemHeart.h"
#include "ItemDash.h"
#include "ItemMagic.h"
#include "MagicCollision.h"
#include "Enemy.h"
#include "Fade.h"

//CollisionObjectを使用するために、ファイルをインクルードする。
#include "collision/CollisionObject.h"
//EffectEmitterを使用するために、ファイルをインクルードする。
#include "graphics/effect/EffectEmitter.h"


bool Player::Start()
{
	//アニメーションのロード
	animationClips[enAnimationClip_Idle].Load("Assets/animData/jackie/idle2.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Run].Load("Assets/animData/jackie/run.tka");
	animationClips[enAnimationClip_Run].SetLoopFlag(true);
	animationClips[enAnimationClip_FastRun].Load("Assets/animData/jackie/fastrun.tka");
	animationClips[enAnimationClip_FastRun].SetLoopFlag(true);
	animationClips[enAnimationClip_Magic].Load("Assets/animData/jackie/magic.tka");
	animationClips[enAnimationClip_Magic].SetLoopFlag(false);
	animationClips[enAnimationClip_Catch].Load("Assets/animData/jackie/catch.tka");
	animationClips[enAnimationClip_Catch].SetLoopFlag(true);
	//animationClips[enAnimationClip_Damage].Load("Assets/animData/jackie/receivedamage.tka");
	//animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	animationClips[enAnimationClip_Down].Load("Assets/animData/jackie/down.tka");
	animationClips[enAnimationClip_Down].SetLoopFlag(false);

	//Jackieモデルを読み込む。
	m_modelRender.Init("Assets/modelData/human/jackie.tkm", animationClips, enAnimationClip_Num, enModelUpAxisZ, true);
	m_modelRender.SetScale({2.0f,2.0f,2.0f});
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetShadowCasterFlag(true);
	m_modelRender.Update();
	//キャラコンを初期化する。
	m_characterController.Init(20.0f, 90.0f, m_position);

	//アニメーションイベント
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//ボーン
	m_PunchBoneId_R = m_modelRender.FindBoneID(L"mixamorig1:RightHand");

	m_gameOverRender.Init("Assets/sprite/kari/Gameover.dds", 1980.0f, 1080.0f);
	m_downTextRender.Init("Assets/sprite/life/Down_text.dds", 1980.0f, 1080.0f);
	m_downLife1Render.Init("Assets/sprite/life/life1.dds", 1980.0f, 1080.0f);
	m_downLife2Render.Init("Assets/sprite/life/life1.dds", 1980.0f, 1080.0f);
	m_downLife3Render.Init("Assets/sprite/life/life1.dds", 1980.0f, 1080.0f);

	//各クラスのFindGO
	m_dash = FindGO<ItemDash>("dash");
	m_magic = FindGO<ItemMagic>("magic");
	m_game = FindGO<Game>("game");
	m_enemys = FindGOs<Enemy>("enemy");

	m_fade = FindGO<Fade>("fade");

	return true;
}

Player::Player()
{
	
}

Player::~Player()
{

}

//更新処理。
void Player::Update()
{
	if (m_game->GetState() == Game::enGameState_Game) {
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

	}
	//アニメーションの再生。
	PlayAnimation();

	//wchar_t wcsbuf[256];
	//swprintf_s(wcsbuf, 256, L"%d",m_gemCount);
	////取得個数
	////表示するテキストを設定。
	//fontRender.SetText(wcsbuf);
	////フォントの位置を設定。
	//fontRender.SetPosition(Vector3(-900.0f, 500.0f, 0.0f));
	////フォントの大きさを設定。
	//fontRender.SetScale(2.0f);
	//swprintf_s(wcsbuf, 256, L"%.3f", m_game->GetTimer());
	////取得個数
	////表示するテキストを設定。
	//fontRender1.SetText(wcsbuf);
	////フォントの位置を設定。
	//fontRender1.SetPosition(Vector3(-900.0f, 350.0f, 0.0f));
	////フォントの大きさを設定。
	//fontRender1.SetScale(2.0f);

	//モデルの更新。
	m_modelRender.Update();
}

//遠投攻撃処理
void Player::MakeMagicCollision()
{
	MagicCollision* magicCollision = NewGO<MagicCollision>(0);
	Vector3 magicCollisionPos = m_position;
	//座標をプレイヤーの少し前に設定する。
	magicCollisionPos += m_forward * 50.0f;
	//座標の位置をあげる
	magicCollisionPos.y += 70.0f;
	magicCollision->SetSpeed(m_forward);
	//座標を設定
	magicCollision->SetPosition(magicCollisionPos);
	magicCollision->SetRotation(m_rotation);

}

//つかみ判定
void Player::Catch()
{
	if (m_notCatchTimer > 0.0f) 
	{
		m_notCatchTimer -= GameTime().GetFrameDeltaTime();
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




	//if (m_playerState == enPlayerState_Run) {
	//	//左スティックの入力量と120.0fを乗算。
	//	right *= stickL.x * 350.0f;
	//	forward *= stickL.y * 350.0f;
	//}
	//if(m_playerState == enPlayerState_FastRun) {
	//	//左スティックの入力量と190.0fを乗算。
	//	right *= stickL.x * 450.0f;
	//	forward *= stickL.y * 450.0f;
	//}


	////移動速度にスティックの入力量を加算する。
	//m_moveSpeed += right + forward;


	/////////////////////////////////



	//移動速度にスティックの入力量を加算する。
	m_moveSpeed += right * stickL.x * 350.0f;
	m_moveSpeed += forward * stickL.y * 350.0f;

	//ダッシュ状態なら速度を1.6倍にする。
	if (m_playerState == enPlayerState_FastRun) {

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
		m_timer -= GameTime().GetFrameDeltaTime();
	}
	else 
	{
		m_timer = 0.0f;
	}
	ProcessState();
}

//パンチステート
void Player::PunchState()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する。
		ProcessState();
	}
}

//遠投攻撃ステート
void Player::MagicState()
{
	if(m_modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する。
		ProcessState();
	}
}

//ダウンステート
void Player::DownState()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		if (m_downCount == 0)
		{
			m_gameOver = true;
		}
		else {
			m_downScreen = true;
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				m_death = false;
				m_downScreen = false;
				m_playerState = enPlayerState_Idle;
			}
		}
	}
}

//ダメージステート
//void Player::DamageState()
//{
//	if (m_modelRender.IsPlayingAnimation() == false)
//	{
//		m_isUnderDamage = false;
//		//ステートを遷移する。
//		ProcessState();
//	}
//}

//キャッチステート
void Player::CatchState()
{
	ProcessState();
}

//ステート遷移
void Player::ProcessState()
{
	if (m_playerState != enPlayerState_Catch) {
		////Yボタンが押されたら。
		//if (g_pad[0]->IsTrigger(enButtonY) && m_magicCount > 0)
		//{
		//	//カウント-1
		//	m_magicCount -= 1;
		//	//遠投攻撃ステートに移行する。
		//	m_playerState = enPlayerState_Magic;

		//	return;
		//}
		
		//Bボタンが押されたら。
		if (g_pad[0]->IsTrigger(enButtonB) && m_fastRun == false && m_dashCount > 0)
		{
			//カウント-1
			m_dashCount -= 1;
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

	//xとzの移動速度が無かったら(スティックの入力が無かったら)。
	else
	{
		//ステートを待機にする。
		m_playerState = enPlayerState_Idle;
	}
	//ダウンしたら。
	if (m_death == true) 
	{
		m_downCount -= 1;
		m_playerState = enPlayerState_Down;
	}
	
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
		//Punchステートの時。
	case enPlayerState_Punch:
		//攻撃ステートのステート遷移処理。
		PunchState();
		break;
		//魔法攻撃ステートの時。
	case enPlayerState_Magic:
		//魔法攻撃ステートのステート遷移処理。
		MagicState();
		break;
		//被ダメージ時ステートの時。
	//case enPlayerState_ReceiveDamage:
	//	//被ダメージ時ステートのステート遷移処理。
	//	DamageState();
	//	break;
		//ダウンステートの時。
	case enPlayerState_Down:
		//ダウンステートのステート遷移処理。
		DownState();
		break;
	case enPlayerState_Catch:
		CatchState();
		break;
		//クリアステートの時。
	//case enPlayerState_Clear:
	//	//クリアステートのステート遷移処理。
	//	ClearState();
	//	break;
	}
}

//アニメーションの再生。
void Player::PlayAnimation()
{
	//switch文。
	switch (m_playerState) {
	//プレイヤーステートが0(Idle)だったら。
	case enPlayerState_Idle:
		//Idle
		m_modelRender.PlayAnimation(enAnimationClip_Idle,0.3f);
		break;
	//プレイヤーステートが1(Run)だったら。
	case enPlayerState_Run:
		//Run
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.3f);
		break;
	//プレイヤーステートが2(FastRun)だったら。
	case enPlayerState_FastRun:
		//FastRun
		m_modelRender.PlayAnimation(enAnimationClip_FastRun, 0.3f);
		break;
	//プレイヤーステートが4(Magic)だったら。
	case enPlayerState_Magic:
		//Magic
		m_modelRender.PlayAnimation(enAnimationClip_Magic, 0.3f);
		break;
	//プレイヤーステートが5(Heal)だったら。
	case enPlayerState_Catch:
		//Catch
		m_modelRender.PlayAnimation(enAnimationClip_Catch, 0.3f);
		break;
	//case enPlayerState_ReceiveDamage:
	//	//Damage
	//	m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.3f);
	//	break;
	case enPlayerState_Down:
		//Down
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.3f);
		break;
	default:
		break;
	}
}

//アニメーションイベント関数
void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//キーの名前が「magic_attack」の時。
	if (wcscmp(eventName, L"magic_attack") == 0) {
		//magicを作成する。
	MakeMagicCollision();
	}
}

//描画処理。
void Player::Render(RenderContext& rc)
{
	//ユニティちゃんを描画する。
	m_modelRender.Draw(rc);
	fontRender.Draw(rc);
	fontRender1.Draw(rc);
	fontRender2.Draw(rc);
	fontRender3.Draw(rc);
	if (m_downScreen == true) {
		m_downTextRender.Draw(rc);
	}
	if (m_gameOver == true)
	{
		m_gameOverRender.Draw(rc);
	}
}

//memo
/**/