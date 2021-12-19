//#include <Random>

#include "stdafx.h"
#include "Item.h"
#include "Player.h"
#include "ItemHeart.h"
#include "ItemMagic.h"
#include "ItemDash.h"

//EffectEmitterを使用するために、ファイルをインクルードする。
#include "graphics/effect/EffectEmitter.h"

Item::Item()
{
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/effect/kirakira_big.efk");
	m_player = FindGO<Player>("player");
}

//void Item::SpawnItem()
//{
//	for (int i = 1; i++; i <= 10) {
//		ran = rand() % 2;
//		if (itemNum == i)
//		{
//			if (ran = 0) {
//
//			}
//		}
//	}
//}

bool Item::GetItem(Vector3 position)
{	
	Vector3 diff = m_player->GetPosition() - position;
	if(diff.Length()<73.0f){
		PlayEffectandSE(position);
		return true;
	}
	else
	{
		return false;
	}
}

void Item::PlayEffectandSE(Vector3 position)
{
	///////効果音の再生///////


	/////エフェクトの再生/////
	//取得エフェクトを発生させる。
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	//ResistEffect関数で指定した番号を指定する。
	effectEmitter->Init(0);
	//大きさを設定する。
	effectEmitter->SetScale(Vector3::One * 11.0f);
	Vector3 effectPosition = position;
	//座標を少し上にする。
	effectPosition.y += 70.0f;
	//座標を設定する。
	effectEmitter->SetPosition(effectPosition);
	//エフェクトを再生する。
	effectEmitter->Play();

}
