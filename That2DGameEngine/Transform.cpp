#include "Transform.h"
#include "GameObject.h"

const glm::vec2& that::Transform::GetWorldPosition()
{
	// If the local position has been changed, recalculate the world position
	if (m_HasChanged) UpdateTransform();

	return m_WorldPosition;
}

void that::Transform::UpdateTransform()
{
	const auto pParent{ GetOwner()->GetParent() };

	m_HasChanged = false;

	// If no parent exist, use the local position as world position
	if (!pParent)
	{
		m_WorldPosition = m_LocalPosition;
		m_WorldRotation = m_LocalRotation;
		return;
	}

	const auto pParentTransform{ pParent->GetTransform() };

	// If no parent transform exist, use the local position as world position
	if (!pParentTransform)
	{
		m_WorldPosition = m_LocalPosition;
		m_WorldRotation = m_LocalRotation;
		return;
	}

	// Calculate the world position using the position of the parent
	const glm::vec2& parentPos{ pParentTransform->GetWorldPosition() };
	const float parentRot{ pParentTransform->GetWorldRotation(false) };
	const float cosAngle{ cosf(parentRot) };
	const float sinAngle{ sinf(parentRot) };

	m_WorldPosition.x = parentPos.x + m_LocalPosition.x * cosAngle - m_LocalPosition.y * sinAngle;
	m_WorldPosition.y = parentPos.y + m_LocalPosition.x * sinAngle + m_LocalPosition.y * cosAngle;

	m_WorldRotation = parentRot + m_LocalRotation;
}

void that::Transform::SetWorldPosition(const glm::vec2& position)
{
	m_LocalPosition += position - GetWorldPosition();
	EnableChangedFlag();
}

void that::Transform::SetWorldPosition(float x, float y)
{
	const auto& worldPos{ GetWorldPosition() };
	m_LocalPosition.x += x - worldPos.x;
	m_LocalPosition.y += y - worldPos.y;

	EnableChangedFlag();
}

void that::Transform::SetLocalPosition(float x, float y)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;

	EnableChangedFlag();
}

void that::Transform::SetLocalPosition(const glm::vec2& position)
{
	m_LocalPosition = position;

	EnableChangedFlag();
}

void that::Transform::Translate(float x, float y)
{
	m_LocalPosition.x += x;
	m_LocalPosition.y += y;

	EnableChangedFlag();
}

float that::Transform::GetLocalRotation(bool isDegrees) const
{
	if (!isDegrees) return m_LocalRotation;

	return glm::degrees(m_LocalRotation);
}

float that::Transform::GetWorldRotation(bool isDegrees)
{
	// If the local position has been changed, recalculate the world position
	if (m_HasChanged) UpdateTransform();

	if (!isDegrees) return m_WorldRotation;

	return glm::degrees(m_WorldRotation);
}

void that::Transform::SetLocalRotation(float rotation, bool isDegrees)
{
	if (!isDegrees) m_LocalRotation = rotation;
	else m_LocalRotation = glm::radians(rotation);

	EnableChangedFlag();
}

void that::Transform::SetWorldRotation(float rotation, bool isDegrees)
{
	const float worldRot{ GetWorldRotation() };
	const float newRotation{ isDegrees ? glm::radians(rotation) : rotation };

	m_LocalRotation += newRotation - worldRot;

	EnableChangedFlag();
}

void that::Transform::Rotate(float angle, bool isDegrees)
{
	const float curAngle{ isDegrees ? glm::radians(angle) : angle };

	m_LocalRotation += curAngle;

	EnableChangedFlag();
}

void that::Transform::EnableChangedFlag()
{
	m_HasChanged = true;

	// Get the owner of this transform
	const auto pOwner{ GetOwner() };
	if (!pOwner) return;

	// Change the HasChanged flag of every child
	const auto& pChildren{ pOwner->GetChildren() };
	for (const auto& pChild : pChildren)
	{
		const auto pTransform{ pChild->GetTransform() };
		if (!pTransform) continue;

		pTransform->EnableChangedFlag();
	}
}
