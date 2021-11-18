#include "stdafx.h"
#include "Item.h"
#include "Player.h"
#include "ItemHeart.h"
#include "ItemPunchUp.h"
#include "ItemMagic.h"
#include "ItemDash.h"

//EffectEmitterを使用するために、ファイルをインクルードする。
#include "graphics/effect/EffectEmitter.h"

Item::Item()
{
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/effect/kirakira_big.efk");
	m_player = FindGO<Player>("player");
}

bool Item::GetItem(Vector3 position)
{	
	//playerPosition = m_player->GetPosition();
	//playerPosition = m_game->GetPosition();

	Vector3 diff = m_player->GetPosition() - position;
	if(diff.Length()<65.0f){
		PlayEffectandSE(position);
		return true;
	}
	else
	{
		return false;
	}
}

void Item::StoreBuy()
{
	switch (m_enBuyState)
	{
	case EnBuyState_Heart:
		if (m_heart->ServeCount() < 3) {
			getCount = m_heart->ServeCount();
			heartStock -= 1;
			getCount += 1;
			m_heart->SetCount(getCount);
			getCount = 0;
		}
		else {
			break;
		}
		if (heartStock == 0)
		{
			break;
		}
		break;
	case EnBuyState_PunchUp:
		if (m_punchUp->ServeCount() < 3) {
			getCount = m_punchUp->ServeCount();
			punchUpStock -= 1;
			m_punchUp->SetCount(getCount);
			getCount = 0;
		}
		else {
			break;
		}
		if (punchUpStock == 0)
		{
			break;
		}
		break;
	case EnBuyState_Magic:
		if (m_magic->ServeCount() < 3) {
			getCount = m_magic->ServeCount();
			magicStock -= 1;
			m_magic->SetCount(getCount);
			getCount = 0;
		}
		else {
			break;
		}
		if (magicStock == 0)
		{
			break;
		}
	case EnBuyState_Dash:
		if (m_dash->ServeCount() < 3) {
			getCount = m_dash->ServeCount();
			magicStock -= 1;
			m_magic->SetCount(getCount);
			getCount = 0;
		}
		else {
			break;
		}
		if (magicStock == 0)
		{
			break;
		}
	case EnBuyState_NotBuy:
		break;
	}
	return;
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
