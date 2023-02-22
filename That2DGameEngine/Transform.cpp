#include "Transform.h"

that::Transform::Transform(std::weak_ptr<GameObject> pParent)
	: Component{ pParent }
{
}

void that::Transform::SetPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
}

void that::Transform::SetPosition(const glm::vec2 position)
{
	m_Position = position;
}
