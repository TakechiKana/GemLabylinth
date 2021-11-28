#include "stdafx.h"
#include "MagicCollision.h"

#include "Player.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//CollisionObject���g�p�������̂ŁA�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"

//EffectEmitter���g�p�������̂ŁA�t�@�C�����C���N���[�h����B
#include "graphics/effect/EffectEmitter.h"

MagicCollision::MagicCollision()
{

}

MagicCollision::~MagicCollision()
{
	//�R���W�����I�u�W�F�N�g���폜����B
	DeleteGO(m_collisionObject);
	////�G�t�F�N�g�̍Đ����~����B
	//m_effectEmitter->Stop();
	////�G�t�F�N�g���폜����B
	//DeleteGO(m_effectEmitter);
}

bool MagicCollision::Start()
{

	m_player = FindGO<Player>("player");

	Vector3 collisionObject = m_position;

	m_collisionObject = NewGO< CollisionObject>(0);
	//����̃R���W�����쐬
	m_collisionObject->CreateSphere(
		collisionObject,
		Quaternion::Identity,
		30.0f
	);
	//�R���W�����̖��O��ݒ�
	m_collisionObject->SetName("player_magic");
	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	m_collisionObject->SetIsEnableAutoDelete(false);

	return true;
}

void MagicCollision::Move()
{
	//�ړ����x���v�Z�B
	m_position += m_moveSpeed * 15.0f;
}

void MagicCollision::Update()
{
	Move();
	m_collisionObject->SetPosition(m_position);

	//�^�C�}�[
	m_timer += g_gameTime->GetFrameDeltaTime();


	//�^�C�}�[��0.7f�ȏゾ������B
	if (m_timer >= 0.9f)
	{
		//���g���폜����B
		DeleteGO(this);
	}
}