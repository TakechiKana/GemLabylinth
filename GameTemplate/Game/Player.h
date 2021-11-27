#pragma once


//クラス宣言
class Collision;
class ItemDash;
class ItemHeart;
class ItemMagic;
class ItemPunchUp;

 
//プレイヤークラス。
class Player : public IGameObject
{
public:
	//ステート
	enum EnPlayerState {
		enPlayerState_Idle,					//待機。
		enPlayerState_Run,					//走る。
		enPlayerState_FastRun,				//速く走る。
		enPlayerState_Punch,				//攻撃
		enPlayerState_PunchUp,				//攻撃力アップ
		enPlayerState_Magic,				//遠投攻撃。
		enPlayerState_Healing,				//回復。
		enPlayerState_ReceiveDamage,		//ダメ―ジ受けた。
		enPlayerState_Down,					//HPが0。
		//enPlayerState_Clear					//クリアー。
	};

	bool Start();
	Player();
	~Player();

	float GetHealth()
	{
		return m_health;
	}

	bool GetState()const
	{
		return m_fastRun;
	}

	//ダッシュタイマー情報を渡す関数
	float GetDashTimer() const
	{
		return m_timer;
	}
	//ダッシュカウント
	void GetDashCount()
	{
		m_dashCount += 1;
	}
	//ハートカウント
	void GetHeartCount()
	{
		m_heartCount += 1;
	}
	//マジックカウント
	void GetMagicCount()
	{
		m_magicCount += 1;
	}
	void GetPunchUpCount()
	{
		m_punchupCount += 1;
	}

	//ポジションを渡す関数
	const Vector3 GetPosition() const
	{
		return m_position;
	}
	//ポジションをもらう
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	//回転設定
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	//移動速度を渡す関数
	Vector3& SetMoveSpeed(const Vector3& movespeed)
	{
		m_moveSpeed = movespeed;
	}
	//
	const Vector3 GetForward() const
	{
		return m_forward;
	}

	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& rc);
	//パンチ
	void Punch();
	//パンチコリジョン
	void MakePunchCollision();
	//遠投攻撃コリジョン
	void MakeMagicCollision();
	//コリジョンの判定
	void Collision();
	//体力、ダメージ、回復
	void Health();
	//アニメーションイベント用関数
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//移動処理。
	void Move();
	//回転処理。
	void Rotation();
	//ステート管理。
	void ManageState();
	//ステート遷移
	void ProcessState();
	//アイドルステート
	void IdleState();
	//ランステート
	void RunState();
	//ファストランステート
	void FastRunState();
	//パンチステート
	void PunchState();
	//遠投攻撃ステート
	void MagicState();
	//回復ステート
	void HealState();
	//ダメージステート
	void DamageState();
	//ダウンステートの時
	void DownState();
	////クリアステート
	//void ClearState();

	//アニメーションの再生。
	void PlayAnimation();
	
	bool IsEnableMove() const
	{
		return m_playerState != enPlayerState_Healing &&
			m_playerState != enPlayerState_Magic &&
			m_playerState != enPlayerState_ReceiveDamage &&
			m_playerState != enPlayerState_Punch &&
			m_playerState != enPlayerState_PunchUp

			/*&&
			m_playerState != enPlayerState_Down &&
			m_playerState != enPlayerState_Clear*/;
	}

private:
	//メンバ変数。
	ModelRender m_modelRender;	//モデルレンダ―。
	Vector3 m_position;			//座標。
	enum EnAnimationClip {		//アニメーション。
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_FastRun,
		enAnimationClip_Punch,
		enAnimationClip_Magic,
		enAnimationClip_Heal,
		enAnimationClip_Damage,
		enAnimationClip_Down,
		enAnimationClip_Num,
	};
	AnimationClip animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	CharacterController m_characterController;				//キャラクターコントローラー。
	Vector3 m_moveSpeed;									//移動速度。
	Vector3 m_forward;										//前方方向ベクトル
	Quaternion m_rotation;									//クォータニオン。
	int m_playerState = enPlayerState_Idle;					//プレイヤーステート
	int m_health = 5;										//HP
	float m_timer = 0.0f;									//ダッシュタイム
	float m_timer1 = 0.0f;									//クールタイム
	bool m_downState = false;								//ダウンの判定
	bool m_isUnderAttack = false;							//攻撃中の判定
	bool m_fastRun = false;									//速度アップ中の判定
	int m_PunchBoneId = -1;									//ボーンID

	int m_heartCount = 0;
	int m_dashCount = 0;
	int m_magicCount = 0;
	int m_punchupCount = 0;

	bool m_death = false;

	//各クラスの変数
	ItemDash* m_dash;
	ItemHeart* m_heart;
	ItemMagic* m_magic;
	ItemPunchUp* m_punchUp;

	SpriteRender m_spriteRender;

	FontRender fontRender;
	FontRender fontRender1; 
	FontRender fontRender2; 
	FontRender fontRender3;
};
