#include "Transform.h"
#include "GameObject.h"

void that::Transform::UpdateTransform()
{
	// Disable the dirty flag
	m_HasChanged = false;

	// Retrieve the parent object
	const auto pParent{ GetOwner()->GetParent() };

	// If no parent exist, use the local position as world position
	if (!pParent)
	{
		m_WorldPosition = m_LocalPosition;
		m_WorldRotation = m_LocalRotation;
		m_WorldScale = m_LocalScale;
		return;
	}

	// Retrieve the parent transform and its properties
	const auto pParentTransform{ pParent->GetTransform() }; 
	const glm::vec2& parentPos{ pParentTransform->GetWorldPosition() };
	const glm::vec2& parentScale{ pParentTransform->GetWorldScale() };
	const float parentRot{ pParentTransform->GetWorldRotation(false) };

	// Calculate the world position
	const float cosAngle{ cosf(parentRot) };
	const float sinAngle{ sinf(parentRot) };
	m_WorldPosition.x = parentPos.x + m_LocalPosition.x * cosAngle * parentScale.x - m_LocalPosition.y * sinAngle * parentScale.y;
	m_WorldPosition.y = parentPos.y + m_LocalPosition.x * sinAngle * parentScale.x + m_LocalPosition.y * cosAngle * parentScale.y;

	// Calculate the world rotation
	m_WorldRotation = parentRot + m_LocalRotation;

	// Calculate the world scale
	m_WorldScale = parentScale * m_LocalScale;
}

const glm::vec2& that::Transform::GetLocalPosition() const
{
	return m_LocalPosition;
}

const glm::vec2& that::Transform::GetWorldPosition()
{
	// If the local position has been changed, recalculate the world position
	if (m_HasChanged) UpdateTransform();

	return m_WorldPosition;
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

void that::Transform::SetWorldPosition(const glm::vec2& position)
{
	SetWorldPosition(position.x, position.y);
}

void that::Transform::SetWorldPosition(float x, float y)
{
	// Retrieve the parent object
	const auto pParent{ GetOwner()->GetParent() };

	// If no parent exist, use the local position as world position
	if (!pParent)
	{
		m_LocalPosition.x = x;
		m_LocalPosition.y = y;
	}
	else
	{
		Transform* pParentTransform{ pParent->GetTransform() };
		const auto& parentPosition{ pParentTransform->GetWorldPosition() };
		const auto& parentScale{ pParentTransform->GetWorldScale() };
		const float parentRot{ pParentTransform->GetWorldRotation(false) };

		const float cosAngle = std::cos(-parentRot);
		const float sinAngle = std::sin(-parentRot);

		const float displacementX{ x - parentPosition.x };
		const float displacementY{ y - parentPosition.y };

		m_LocalPosition.x = (displacementX * cosAngle - displacementY * sinAngle) / parentScale.x;
		m_LocalPosition.y = (displacementX * sinAngle + displacementY * cosAngle) / parentScale.y;
	}

	EnableChangedFlag();
}

void that::Transform::Translate(float x, float y)
{
	m_LocalPosition.x += x;
	m_LocalPosition.y += y;

	EnableChangedFlag();
}

void that::Transform::Translate(const glm::vec2& displacement)
{
	m_LocalPosition += displacement;

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

const glm::vec2& that::Transform::GetLocalScale() const
{
	return m_LocalScale;
}

const glm::vec2& that::Transform::GetWorldScale()
{	
	// If the local position has been changed, recalculate the world position
	if (m_HasChanged) UpdateTransform();

	return m_WorldScale;
}

void that::Transform::SetLocalScale(float scale)
{
	m_LocalScale.x = scale;
	m_LocalScale.y = scale;

	EnableChangedFlag();
}

void that::Transform::SetLocalScale(const glm::vec2& scale)
{
	m_LocalScale = scale;

	EnableChangedFlag();
}

void that::Transform::SetWorldScale(float scale)
{
	const auto& worldScale{ GetWorldScale() };

	m_LocalScale.x = scale / worldScale.x;
	m_LocalScale.y = scale / worldScale.y;

	EnableChangedFlag();
}

void that::Transform::SetWorldScale(const glm::vec2& scale)
{
	const auto& worldScale{ GetWorldScale() };

	m_LocalScale.x = scale.x / worldScale.x;
	m_LocalScale.y = scale.y / worldScale.y;

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
