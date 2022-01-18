#pragma once
//�΂˃J�������g�p�������ꍇ�́ASpringCamera���C���N���[�h����B
#include "camera/SpringCamera.h"

//�N���X�錾�B
class Player;
class Game;
class Enemy;

/// <summary>
/// �J�����B
/// </summary>
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	/// <summary>
	/// ���W�Ǝ��_���X�V����B
	/// </summary>
	void UpdatePositionAndTarget();

private:
	Game* m_game = nullptr;
	Player* m_player = nullptr;								//�v���C���[�B
	Enemy* m_enemy = nullptr;								//�G�l�~�[�B
	Vector3				m_toCameraPos = Vector3::One;		//�����_���王�_�Ɍ������x�N�g���B
	Vector3				target;
	Vector3 m_position;
	Vector3 m_moveSpeed;
	SpringCamera		m_springCamera;						//�΂˃J�����B
};
