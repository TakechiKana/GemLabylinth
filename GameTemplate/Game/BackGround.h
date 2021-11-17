#pragma once
class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	void Render(RenderContext& rc);

    //m_を付ける
    //できれば、メンバ変数はprivateに。(詳細は中谷とかに聞いて)
private:
	ModelRender m_modelRender;
	PhysicsStaticObject m_physicsStaticObject;
	Vector3 m_position;
	Vector3 m_scale;
	Quaternion m_rotation;
};
