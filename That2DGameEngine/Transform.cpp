#include "Transform.h"
#include "GameObject.h"

void that::Transform::SetWorldPosition(const glm::vec2& position)
{
	m_LocalPosition += position - GetWorldPosition();
	EnableChangedFlag();
}

const glm::vec2& that::Transform::GetWorldPosition()
{
	// If the local position has been changed, recalculate the world position
	if (m_HasChanged) UpdateWorldPosition();

	return m_WorldPosition;
}

void that::Transform::UpdateWorldPosition()
{
	const auto pParent{ GetOwner()->GetParent() };

	// If no parent exist, use the local position as world position
	if (!pParent)
	{
		m_WorldPosition = m_LocalPosition;
		return;
	}

	const auto pParentTransform{ pParent->GetTransform() };

	// If no parent transform exist, use the local position as world position
	if (!pParentTransform)
	{
		m_WorldPosition = m_LocalPosition;
		return;
	}

	// Calculate the world position using the position of the parent
	m_WorldPosition = pParentTransform->GetWorldPosition() + m_LocalPosition;

	m_HasChanged = false;
}

void that::Transform::SetWorldPosition(float x, float y)
{
	const glm::vec2& worldPos{ GetWorldPosition() };

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
