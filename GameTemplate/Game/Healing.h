#pragma once
#pragma once
#include "sound/SoundSource.h"

class Player;
//�񕜃N���X�B
class Healing : public IGameObject
{
public:
	bool Start();
	Healing();
	~Healing();
	//�X�V�����B
	void Update();
	//�`�揈���B
	void Render(RenderContext& rc);
	//��]�����B
	void Rotation();
	//�v���C���[�̉񕜏���
	void HealProcess();
	//�G�t�F�N�g�̍Đ�
	//void Effect();
private:
	//�����o�ϐ��B
	ModelRender modelRender;	//���f�������_�\�B
	Vector3 position;		//���W�B
	Vector3 firstPosition;		//�ŏ��̍��W�B
	Quaternion rotation;	//�N�H�[�^�j�I���B
	Player* m_player;		//�v���C���[�B

};
