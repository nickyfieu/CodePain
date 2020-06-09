#include "CodePainPCH.h"
#include "Transform.h"

cp::Transform::Transform(const float x, const float y, const float z)
	: BaseComponent( cp::ComponentType::_Transform )
{
	SetPosition(x, -y, z);
}

cp::Transform::~Transform()
{
}

void cp::Transform::Update(const float)
{
}

void cp::Transform::FixedUpdate(const float)
{
}

void cp::Transform::DebugDraw() const
{
}

void cp::Transform::Draw() const
{
}

void cp::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = -y;
	m_Position.z = z;
}

void cp::Transform::Translate(float x, float y, float z)
{
	m_Position.x += x;
	m_Position.y += -y;
	m_Position.z += z;
}
