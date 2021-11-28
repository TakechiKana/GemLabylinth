#pragma once

class Player;

class MagicCollision : public IGameObject
{
public:
	MagicCollision();
	~MagicCollision();
	bool Start();
	void Move();
	void Update();
	//���W��ݒ�
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	//��]��ݒ�
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	//�ړ����x��ݒ�
	void SetSpeed(const Vector3& speed)
	{
		m_moveSpeed = speed;
	}
private:
	Vector3 m_position;
	Vector3 m_moveSpeed;
	Quaternion m_rotation;
	float m_timer = 0.0f;
	CollisionObject* m_collisionObject = nullptr;   
	Player* m_player = nullptr;
};

