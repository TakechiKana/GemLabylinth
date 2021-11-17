#include "stdafx.h"
#include "Healing.h"
#include "Player.h"

//#include "graphics/effect/effectemitter.h"
//#include "sound/soundengine.h"


Healing::Healing()
{
	//宝石のモデルを読み込む。
	modelRender.Init("Assets/modelData/jewelry/jewelry_red.tkm");

	player = FindGO<Player>("player");
}

Healing::~Healing()
{

}

bool Healing::Start()
{
	//エフェクトのロード
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/kirakira.efk");

	return true;
}

void Healing::Update()
{

	//回転処理。
	Rotation();
	//個数判定
	Capture();

	//絵描きさんの更新処理。
	modelRender.Update();

}

//void Healing::Effect()
//{
//	//攻撃エフェクトを発生させる。
//	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
//	//ResistEffect関数で指定した番号を指定する。
//	effectEmitter->Init(1);
//	//大きさを設定する。
//	effectEmitter->SetScale(Vector3::One * 11.0f);
//	Vector3 effectPosition = position;
//	//座標を少し上にする。
//	effectPosition.y += 70.0f;
//	//座標を設定する。
//	effectEmitter->SetPosition(effectPosition);
//	Quaternion rotation;
//	//プレイヤーの回転。
//	rotation = rotation;
//	//回転させる。
//	rotation.AddRotationDegY(360.0f);
//	rotation.AddRotationDegZ(180.0f);
//	//回転を設定する。
//	effectEmitter->SetRotation(rotation);
//	//エフェクトを再生する。
//	effectEmitter->Play();
//}

void Healing::HealProcess()
{
	//プレイヤーから☆に向かうベクトルを計算。
	Vector3 diff = m_player->m_position - position;
	//プレイヤーが5個以上持っているときはtrue
	if (player->redCount >= 5)
	{
		Playercapacity = true;
	}
	//そうじゃないときはfalse
	else
	{
		Playercapacity = false;
	}
	//プレイヤーが5個以上持っていなければ
	if (Playercapacity == false)
	{
		//ベクトルの長さが70.0fより小さかったら。
		if (diff.Length() <= 70.0f)
		{
			player->redCount += 1;

			Effect();

			Kira = NewGO<SoundSource>(0);
			Kira->Init(4);
			Kira->Play(false);

			//自身を削除する。
			DeleteGO(this);
		}
	}
}

//回転処理。
void Healing::Rotation()
{
	//回転を加算する。
	rotation.AddRotationDegY(2.0f);

	//絵描きさんに回転を教える。
	modelRender.SetRotation(rotation);
	modelRender.SetPosition(position);
	modelRender.SetScale({ 2.0f,2.0f,2.0f });
}

void Healing::Render(RenderContext& rc)
{
	//☆を描画する。
	modelRender.Draw(rc);
}