#pragma once

#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"

class Player;

class Enemy : public IGameObject
{
public:
	//エネミーステート
	enum EnEnemyState {
		enEnemyState_Idle,					//待機。
		enEnemyState_Chase,					//追跡。
		enEnemyState_Punch,					//攻撃。
		enEnemyState_Phose,					//殴り後の一時停止
		enEnemyState_Magic,					//遠投攻撃。
		enEnemyState_ReceiveDamage,			//被ダメージ。
	};

public:
	Enemy();
	~Enemy();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	//////////他クラスへ渡すための関数//////////
	//座標を他からもらう
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	//座標を渡す
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	//回転を設定
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	//キャッチの判定を渡す
	const bool GetCatchState() const
	{
		return m_catch;
	}
	const float GetCatchTimer() const
	{
		return m_catchTimer;
	}
private:
	//移動処理
	void Move();
	//追跡処理
	void Chase();
	//回転処理
	void Rotation();
	//当たり判定
	void Collision();
	//ポーズ中の処理
	void Phose();
	//プレイヤー探索
	const bool SearchPlayer() const;
	//アニメーション再生
	void PlayAnimation();
	//アニメーションイベント用関数
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//ステート遷移処理
	void ManageState();
	/// 共通のステート遷移処理。
	void ProcessState();
	/// 待機ステートの遷移処理。
	void IdleState();
	/// 歩きステートの遷移処理。
	void WalkState();
	/// 走りステートの遷移処理。
	void RunState();
	/// 追跡ステートの背遷移処理。
	void ChaseState();
	/// 攻撃ステートの遷移処理。
	void PunchState();
	/// 殴り後一時停止ステートの遷移処理。
	void PhoseState();
	/// 被ダメージステートの遷移処理。
	void DamageState();
	//攻撃できる距離の判定
	void IsCanPunch();

	enum EnAnimationClip {						//アニメーション。
		enAnimationClip_Idle,					//待機アニメーション。
		enAnimationClip_Walk,					//歩きアニメーション。
		enAnimationClip_Run,					//走りアニメーション。
		enAnimationClip_Punch,					//攻撃アニメーション。
		enAnimationClip_Phose,					//攻撃後一時停止アニメーション。
		enAnimationClip_Damage,					//被ダメージアニメーション。
		enAnimationClip_Num,					//アニメーションの数。
	};
	AnimationClip				m_animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	ModelRender					m_modelRender;								//モデルレンダー。
	Vector3						m_position;									//座標。
	Vector3						m_targetPosition;							//ターゲットの座標
	Vector3						m_moveSpeed;								//移動速度。
	Vector3						m_forward = Vector3::AxisZ;					//エネミーの正面ベクトル。
	Vector3						padding = { 20.0f,20.0f,20.0f };			//ターゲットをプレイヤーより少し離す。
	Quaternion					m_rotation;									//回転。
	Vector3						m_scale = Vector3::One;						//大きさ。
	CharacterController			m_charaCon;									//キャラコン。
	EnEnemyState				m_enemyState = enEnemyState_Idle;			//エネミーステート。

	//ナビメッシュ
	TknFile m_tknFile;
	nsAI::NaviMesh m_nvmMesh;
	nsAI::Path m_path;
	nsAI::PathFinding m_pathFiding;
	////

	bool						m_isUnderDamage = false;					//ダメージを受けたか
	bool						m_catch = false;							//プレイヤーを捕まえた？
	float						m_catchTimer = 0.0f;
	Player*						m_player = nullptr;							//プレイヤー。
	int							m_PunchBoneId = -1;							//ぼーんID
	Vector3						m_velocity = Vector3::Zero;					//移動方向
};

