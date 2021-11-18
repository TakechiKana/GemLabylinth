#include "stdafx.h"
#include "Item.h"
#include "Player.h"
#include "ItemHeart.h"
#include "ItemPunchUp.h"
#include "ItemMagic.h"
#include "ItemDash.h"

//EffectEmitter���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
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
	///////���ʉ��̍Đ�///////


	/////�G�t�F�N�g�̍Đ�/////
	//�擾�G�t�F�N�g�𔭐�������B
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	//ResistEffect�֐��Ŏw�肵���ԍ����w�肷��B
	effectEmitter->Init(0);
	//�傫����ݒ肷��B
	effectEmitter->SetScale(Vector3::One * 11.0f);
	Vector3 effectPosition = position;
	//���W��������ɂ���B
	effectPosition.y += 70.0f;
	//���W��ݒ肷��B
	effectEmitter->SetPosition(effectPosition);
	//�G�t�F�N�g���Đ�����B
	effectEmitter->Play();

}
