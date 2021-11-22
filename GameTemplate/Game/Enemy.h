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
		enEnemyState_Magic,					//遠投攻撃。
		enEnemyState_ReceiveDamage,			//被ダメージ。
		enEnemyState_Down,					//ダウン
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
	//HPを他からもらう
	void SetHP(const int hp)
	{
		m_hp = hp;
	}
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

private:
	//移動処理
	void Move();
	//追跡処理
	void Chase();
	//回転処理
	void Rotation();
	//攻撃処理
	void Attack();
	//当たり判定
	void Collision();
	//プレイヤー探索
	const bool SearchPlayer() const;
	//パンチの当たり判定コリジョン生成
	void MakePunchCollision();
	//ステージ2以降実装するなら使用予定
	//void MakeMagic();
	//アニメーション再生
	void PlayAnimation();
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
	/// 魔法攻撃ステートの遷移処理。
	//void MagicState();
	/// 被ダメージステートの遷移処理。
	void DamageState();
	/// ダウンステートの遷移処理。
	void DownState();
	//攻撃できる距離の判定
	const bool IsCanPunch() const;

	enum EnAnimationClip {						//アニメーション。
		enAnimationClip_Idle,					//待機アニメーション。
		enAnimationClip_Walk,					//歩きアニメーション。
		enAnimationClip_Run,					//走りアニメーション。
		enAnimationClip_Punch,					//攻撃アニメーション。
		//enAnimationClip_Magic,				//魔法攻撃アニメーション。
		enAnimationClip_Damage,					//被ダメージアニメーション。
		enAnimationClip_Down,					//ダウンアニメーション。
		enAnimationClip_Num,					//アニメーションの数。
	};
	AnimationClip				m_animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	ModelRender					m_modelRender;								//モデルレンダー。
	Vector3						m_position;									//座標。
	Vector3						m_targetPosition;							//ターゲットの座標
	Vector3						m_moveSpeed;								//移動速度。
	Vector3						m_forward = Vector3::AxisZ;					//エネミーの正面ベクトル。
	Quaternion					m_rotation;									//回転。
	Vector3						m_scale = Vector3::One;						//大きさ。
	CharacterController			m_charaCon;									//キャラコン。
	EnEnemyState				m_enemyState = enEnemyState_Idle;			//エネミーステート。
	TknFile m_tknFile;
	nsAI::NaviMesh m_nvmMesh;
	nsAI::Path m_path;
	nsAI::PathFinding m_pathFiding;
	bool						m_isUnderAttack = false;					//攻撃中か？
	int							m_hp = 0;									//HP。
	Player*						m_player = nullptr;							//プレイヤー。
	float						m_chaseTimer = 0.0f;						//追跡タイマー。
	float						m_idleTimer = 0.0f;
	int							m_PunchBoneId = -1;							//ぼーんID

};

