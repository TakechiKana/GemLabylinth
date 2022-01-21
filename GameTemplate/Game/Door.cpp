#include "stdafx.h"
#include "Door.h"
#include "Player.h"

Door::Door()
{
}

Door::~Door()
{
}

bool Door::Start()
{
	//モデル読み込み
	m_modelRender.Init("Assets/modelData/stage/door.tkm", m_animationClips, enAnimationClip_Num);
	//影を落とさない
	m_modelRender.SetShadowCasterFlag(false);
	//座標を設定する。
	m_modelRender.SetPosition(m_position);
	//アニメーションを読み込む。
	m_animationClips[enAnimationClip_CloseIdle].Load("Assets/animData/door/idle_close.tka");
	m_animationClips[enAnimationClip_CloseIdle].SetLoopFlag(false);
	m_animationClips[enAnimationClip_OpenIdle].Load("Assets/animData/door/idle_open.tka");
	m_animationClips[enAnimationClip_OpenIdle].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Open].Load("Assets/animData/door/open.tka");
	m_animationClips[enAnimationClip_Open].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Close].Load("Assets/animData/door/close.tka");
	m_animationClips[enAnimationClip_Close].SetLoopFlag(false);
	//モデルの更新。
	m_modelRender.Update();
	//当たり判定を作成する。
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	m_player = FindGO<Player>("player");
	return true;
}

void Door::Update()
{
	//アニメーションを再生する。
	PlayAnimation();
	//ステート管理。
	ManageState();

	//モデルの更新。
	m_modelRender.Update();
}
void Door::NotifyOpen()
{
	//オープンステートに遷移する。
	m_doorState = enDoorState_Open;
	//音を再生する。
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(5);
	se->Play(false);
	se->SetVolume(0.7f);
}

void Door::NotifyClose()
{
	//クローズステートに遷移する。
	m_doorState = enDoorState_Close;
	//音を再生する。
	/*SoundSource* se = NewGO<SoundSource>(0);
	se->Init(5);
	se->Play(false);
	se->SetVolume(0.6f);*/
}

void Door::PlayAnimation()
{
	switch (m_doorState)
	{
		//待機ステートの時。
	case enDoorState_CloseIdle:
		//待機アニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_CloseIdle);
		break;
		//待機ステートの時。
	case enDoorState_OpenIdle:
		//待機アニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_OpenIdle);
		break;
		//オープンステートの時。
	case enDoorState_Open:
		//オープンアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_Open);
		m_modelRender.SetAnimationSpeed(0.6f);
		break;
		//クローズステートの時。
	case enDoorState_Close:
		//クローズアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_Close);
		m_modelRender.SetAnimationSpeed(0.6f);
		break;
	default:
		break;
	}
}

void Door::ReleasePhysicsObject()
{
	//当たり判定を開放する。
	m_physicsStaticObject.Release();
}

void Door::CreatePhysicsObject()
{
	//当たり判定を作成する。
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
}

void Door::ProcessOpenStateTransition()
{
	//オープンアニメーションの再生が終了したら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//当たり判定を開放する。
		ReleasePhysicsObject();
		//オープン終わりステートに遷移する。
		m_doorState = enDoorState_OpenIdle;
	}
}

void Door::ProcessCloseStateTransition()
{
	//クローズアニメーションが終了したら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//当たり判定を作成する。
		CreatePhysicsObject();
		//クローズ終わりステートに遷移する。
		m_doorState = enDoorState_CloseIdle;
	}
}

void Door::ManageState()
{
	switch (m_doorState)
	{
		//待機ステートの時。
	case Door::enDoorState_CloseIdle:

		break;
		//オープンステートの時。
	case Door::enDoorState_Open:
		//オープンステートのステート遷移処理。
		ProcessOpenStateTransition();
		break;
		//オープン終わりステートの時。
	case Door::enDoorState_OpenIdle:

		break;
		//クローズステートの時。
	case Door::enDoorState_Close:
		//クローズステートのステート遷移処理。
		ProcessCloseStateTransition();
		break;

	default:
		break;
	}
}

void Door::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}