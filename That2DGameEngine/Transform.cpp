#include "Transform.h"

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
	const auto pParent{ GetParent()->GetParent() };

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

	// If no parent exists, stop here
	const auto pParent{ GetParent() };
	if (!pParent) return;

	// Change the HasChanged flag of every child
	const auto& pChildren{ pParent->GetChildren() };
	for (const auto& pWeakChild : pChildren)
	{
		if (pWeakChild.expired()) continue;

		const auto pChild{ pWeakChild.lock() };

		const auto pTransform{ pChild->GetTransform() };
		if (!pTransform) continue;

		pTransform->EnableChangedFlag();
	}
}
