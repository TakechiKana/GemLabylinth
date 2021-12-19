//#include <Random>

#include "stdafx.h"
#include "Item.h"
#include "Player.h"
#include "ItemHeart.h"
#include "ItemMagic.h"
#include "ItemDash.h"

//EffectEmitter���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
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
