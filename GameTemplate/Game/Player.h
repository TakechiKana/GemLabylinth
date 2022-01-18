#pragma once


//クラス宣言
class Collision;
class ItemDash;
class Game;
class Enemy;
class Fade;

 
//プレイヤークラス。
class Player : public IGameObject
{
public:
	//ステート
	enum EnPlayerState {
		enPlayerState_Idle,					//待機。
		enPlayerState_Run,					//走る。
		enPlayerState_FastRun,				//速く走る。
		enPlayerState_Down,					//HPが0。
		enPlayerState_Catch,				//つかまった。
		enPlayerState_Clear					//クリアー。
	};

	bool Start();
	Player();
	~Player();

	//ダメージ処理
	void RaceiveDamage()
	{
		m_death = true;
	}

	//ダッシュアイテムを拾ったら
	void GetDashCount()
	{
		m_dashCount += 1;
	}
	//ジェムを拾ったら
	void GetGemCount()
	{
		m_gemCount += 1;
	}

	//ポジションを渡す関数
	const Vector3 GetPosition() const
	{
		return m_position;
	}

	//ポジションをクラス外で設定
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	//再スタート地点の設定
	void SetRestartPosition(const Vector3& position)
	{
		m_restartPos = position;
	}

	//回転をクラス外で設定
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	//playerの前方向
	const Vector3 GetForward() const
	{
		return m_forward;
	}
	////クリア
	bool Clear() {
		if (m_gemCount == 178 && m_clear == true)
		{
			return true;
		}
	}
	
	bool IsEnableMove() const
	{
		return m_playerState != enPlayerState_Down &&
			m_playerState != enPlayerState_Catch &&
			m_playerState != enPlayerState_Clear;
	}

private:
	//つかみ判定
	void Catch();
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
	//ダウンステートの時
	void DownState();
	//キャッチステート
	void CatchState();
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& rc);
	//移動処理。
	void Move();
	//回転処理。
	void Rotation();
	//アニメーションの再生。
	void PlayAnimation();
	//ダウン時の残機表示
	//void LifeRender(RenderContext& rc);


	//メンバ変数。
	enum EnAnimationClip //アニメーション。
	{		
		enAnimationClip_Idle,		//待機アニメーション。
		enAnimationClip_Run,		//走りアニメーション。
		enAnimationClip_FastRun,	//ダッシュアイテム時の走りアニメーション。
		enAnimationClip_Catch,		//つかまれたアニメーション。
		enAnimationClip_Down,		//ダウンアニメーション。
		enAnimationClip_Num,		//アニメーション数。
	};
	
	Vector3 m_moveSpeed;									//移動速度。
	Vector3 m_forward;										//前方方向ベクトル
	Vector3 m_position;										//座標。
	Vector3 m_restartPos;									//再スタート地点
	Quaternion m_rotation;									//クォータニオン。
	EnPlayerState m_playerState = enPlayerState_Idle;		//プレイヤーステート
	ModelRender m_modelRender;								//モデルレンダ―。
	AnimationClip animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	CharacterController m_characterController;				//キャラクターコントローラー。

	float m_timer = 0.0f;									//ダッシュタイム
	float m_timer1 = 0.0f;									//マジック削除まで
	float m_notCatchTimer = 0.0f;							//無敵時間

	bool m_fastRun = false;									//速度アップ中の判定

	int m_dashCount = 0;									//ダッシュアイテムカウント
	int m_magicCount = 0;									//マジックアイテムカウント
	int m_gemCount = 0;										//ジェムカウント
	int m_downCount = 3;									//ダウン回数

	bool m_death = false;									//ダウンしたか？
	bool m_downScreen = false;								//ダウン画面
	bool m_clear = false;
	bool m_gameOver = false;

	int	m_PunchBoneId_R= -1;									//BoneID

	//各クラスの変数
	ItemDash* m_dash;
	Game* m_game;
	Fade* m_fade;

	std::vector<Enemy*>  m_enemys;

	SpriteRender m_downTextRender;		//ダウン時テキスト
	SpriteRender m_downLife1Render;		//ダウン時残機１
	SpriteRender m_downLife2Render;		//ダウン時残機２
	SpriteRender m_downLife3Render;		//ダウン時残機３
	SpriteRender m_gameOverRender;		//GameOver


	FontRender fontRender;
	FontRender fontRender1; 
	FontRender fontRender2; 
	FontRender fontRender3;
};
