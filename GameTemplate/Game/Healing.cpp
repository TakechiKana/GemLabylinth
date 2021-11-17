#include "stdafx.h"
#include "Healing.h"
#include "Player.h"

//#include "graphics/effect/effectemitter.h"
//#include "sound/soundengine.h"


Healing::Healing()
{
	//��΂̃��f����ǂݍ��ށB
	modelRender.Init("Assets/modelData/jewelry/jewelry_red.tkm");

	player = FindGO<Player>("player");
}

Healing::~Healing()
{

}

bool Healing::Start()
{
	//�G�t�F�N�g�̃��[�h
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/kirakira.efk");

	return true;
}

void Healing::Update()
{

	//��]�����B
	Rotation();
	//������
	Capture();

	//�G�`������̍X�V�����B
	modelRender.Update();

}

//void Healing::Effect()
//{
//	//�U���G�t�F�N�g�𔭐�������B
//	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
//	//ResistEffect�֐��Ŏw�肵���ԍ����w�肷��B
//	effectEmitter->Init(1);
//	//�傫����ݒ肷��B
//	effectEmitter->SetScale(Vector3::One * 11.0f);
//	Vector3 effectPosition = position;
//	//���W��������ɂ���B
//	effectPosition.y += 70.0f;
//	//���W��ݒ肷��B
//	effectEmitter->SetPosition(effectPosition);
//	Quaternion rotation;
//	//�v���C���[�̉�]�B
//	rotation = rotation;
//	//��]������B
//	rotation.AddRotationDegY(360.0f);
//	rotation.AddRotationDegZ(180.0f);
//	//��]��ݒ肷��B
//	effectEmitter->SetRotation(rotation);
//	//�G�t�F�N�g���Đ�����B
//	effectEmitter->Play();
//}

void Healing::HealProcess()
{
	//�v���C���[���灙�Ɍ������x�N�g�����v�Z�B
	Vector3 diff = m_player->m_position - position;
	//�v���C���[��5�ȏ㎝���Ă���Ƃ���true
	if (player->redCount >= 5)
	{
		Playercapacity = true;
	}
	//��������Ȃ��Ƃ���false
	else
	{
		Playercapacity = false;
	}
	//�v���C���[��5�ȏ㎝���Ă��Ȃ����
	if (Playercapacity == false)
	{
		//�x�N�g���̒�����70.0f��菬����������B
		if (diff.Length() <= 70.0f)
		{
			player->redCount += 1;

			Effect();

			Kira = NewGO<SoundSource>(0);
			Kira->Init(4);
			Kira->Play(false);

			//���g���폜����B
			DeleteGO(this);
		}
	}
}

//��]�����B
void Healing::Rotation()
{
	//��]�����Z����B
	rotation.AddRotationDegY(2.0f);

	//�G�`������ɉ�]��������B
	modelRender.SetRotation(rotation);
	modelRender.SetPosition(position);
	modelRender.SetScale({ 2.0f,2.0f,2.0f });
}

void Healing::Render(RenderContext& rc)
{
	//����`�悷��B
	modelRender.Draw(rc);
}