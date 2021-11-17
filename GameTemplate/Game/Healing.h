#pragma once
#pragma once
#include "sound/SoundSource.h"

class Player;
//回復クラス。
class Healing : public IGameObject
{
public:
	bool Start();
	Healing();
	~Healing();
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& rc);
	//回転処理。
	void Rotation();
	//プレイヤーの回復処理
	void HealProcess();
	//エフェクトの再生
	//void Effect();
private:
	//メンバ変数。
	ModelRender modelRender;	//モデルレンダ―。
	Vector3 position;		//座標。
	Vector3 firstPosition;		//最初の座標。
	Quaternion rotation;	//クォータニオン。
	Player* m_player;		//プレイヤー。

};
