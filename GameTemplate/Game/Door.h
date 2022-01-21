#pragma once
class Player;

class Door : public IGameObject
{
public:
	enum EnDoorState {
		enDoorState_CloseIdle,			//閉の待機状態。
		enDoorState_OpenIdle,			//開の待機状態。
		enDoorState_Open,				//オープン中。
		enDoorState_Close				//クローズ中。
	};
public:
	Door();
	~Door();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// 座標の設定。
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ドアの番号を設定する。
	/// </summary>
	/// <param name="doorNumber">ドアの番号。</param>
	void SetDoorNumber(const int doorNumber)
	{
		m_doorNumber = doorNumber;
	}
	/// <summary>
	/// ドアの番号を取得する。
	/// </summary>
	/// <returns>ドアの番号。</returns>
	const int GetDoorNumber() const
	{
		return m_doorNumber;
	}
	/// <summary>
	/// オープンを通知する。
	/// </summary>
	void NotifyOpen();
	/// <summary>
	/// クローズを通知する。
	/// </summary>
	void NotifyClose();

//メンバ関数
private:
	/// <summary>
	/// アニメーション再生。
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// オブジェクトの当たり判定を開放。
	/// </summary>
	void ReleasePhysicsObject();
	/// <summary>
	/// オブジェクトの当たり判定を作成。
	/// </summary>
	void CreatePhysicsObject();
	/// <summary>
	/// ステート管理。
	/// </summary>
	void ManageState();
	/// <summary>
	/// オープンステート遷移処理。
	/// </summary>
	void ProcessOpenStateTransition();
	/// <summary>
	/// クローズステート遷移処理。
	/// </summary>
	void ProcessCloseStateTransition();

//メンバ変数
private:
	enum EnAnimationClip //アニメーション。
	{
		enAnimationClip_CloseIdle,	//待機アニメーション。(閉じているとき)
		enAnimationClip_OpenIdle,	//待機アニメーション。(開いているとき)
		enAnimationClip_Open,		//オープンアニメーション。
		enAnimationClip_Close,		//クローズアニメーション。
		enAnimationClip_Num,		//アニメーション数。
	};

	Player* m_player;
	ModelRender m_modelRender;
	Vector3 m_position;
	Vector3 m_scale;
	AnimationClip				m_animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	PhysicsStaticObject			m_physicsStaticObject;						//フィジクススタティックオブジェクト。	
	EnDoorState					m_doorState = enDoorState_CloseIdle;		//ドアステート。
	int							m_doorNumber = 0;							//ドアの番号。

};
