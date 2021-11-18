#pragma once

class Player;
class ItemHeart;
class ItemPunchUp;
class ItemMagic;
class ItemDash;

class Item
{
public:
	Item();
	~Item() {}

	//�w���X�e�[�g
	enum EnBuyState {
		EnBuyState_Heart,					//�񕜍w��
		EnBuyState_PunchUp,					//�U���̓A�b�v�w��
		EnBuyState_Magic,					//�����U���w��
		EnBuyState_Dash,					//�ړ����x�A�b�v�w��
		EnBuyState_NotBuy					//��������Ȃ�
	};

	bool GetItem(Vector3 position);
	int GetItemCount(bool m_state, int n)
	{
		if (m_state == true) {
			getCount = n + 1;
		}
		return getCount;
		getCount = 0;
	}
	int ItemUse(bool m_state, int n)
	{
		if (m_state == true) {
			getCount = n - 1;
		}
		return getCount;
		getCount = 0;
	}

	void StoreBuy();

	void PlayEffectandSE(Vector3 position);

private:
	int m_enBuyState = EnBuyState_NotBuy;
	Quaternion m_rotation;
	Vector3 position;
	Player* m_player = nullptr;
	ItemHeart* m_heart;
	ItemPunchUp* m_punchUp;
	ItemMagic* m_magic;
	ItemDash* m_dash;
	bool m_state;
	bool m_state1;
	int heartStock = 5;
	int punchUpStock = 5;
	int magicStock = 5;
	int dashStock = 5;
	int heartCount = 0;
	int punchUpCount = 0;
	int magicCount = 0;
	int dashCount = 0;
	int getCount = 0;
};

