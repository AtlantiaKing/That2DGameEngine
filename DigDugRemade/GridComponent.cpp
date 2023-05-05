#include "GridComponent.h"

#include "GameObject.h"
#include "Transform.h"
#include "GridTransform.h"
#include "GridCollider.h"

#include "SDL.h"
#include "Renderer.h"

void digdug::GridComponent::LateUpdate()
{
	const auto& children{ GetOwner()->GetChildren() };

	std::vector<GridCollider*> pColliders{};
	pColliders.reserve(children.size()); // Reverse some amount of space in advance (there will be more colliders than children.size)

	// Get all colliders in children and grandchildren
	for (auto pChild : GetOwner()->GetChildren())
	{
		auto pCollider{ pChild->GetComponent<GridCollider>() };
		if (pCollider) pColliders.push_back(pCollider);

		for (auto pGrandChild : pChild->GetChildren())
		{
			auto pChildCollider{ pGrandChild->GetComponent<GridCollider>() };
			if (pChildCollider) pColliders.push_back(pChildCollider);
		}
	}

	// Check collision between all colliders found
	for (auto pChild : pColliders)
	{
		const auto& childPos{ pChild->GetOwner()->GetTransform()->GetWorldPosition() };
		for (auto pOther : pColliders)
		{
			if (pChild == pOther) continue;

			const auto& otherPos{ pOther->GetOwner()->GetTransform()->GetWorldPosition() };

			if (DoOverlap(childPos, otherPos))
			{
				pChild->Hit(pOther);
			}
		}
	}
}

const glm::vec2& digdug::GridComponent::GetPivot() const
{
	return GetTransform()->GetWorldPosition();
}

bool digdug::GridComponent::IsValidPosition(const glm::vec2& position)
{
	if (position.x < 0.0f || position.y < 0.0f) return false;

	const int maxGridIdx{ m_GridSize - 1 };
	if (position.x > maxGridIdx * m_StepsPerCell || position.y > maxGridIdx * m_StepsPerCell) return false;

	return true;
}

bool digdug::GridComponent::DoOverlap(const glm::vec2& pos0, const glm::vec2& pos1)
{
	const float l1x{ pos0.x };
	const float l1y{ pos0.y + m_CellSize };
	const float r1x{ pos0.x + m_CellSize };
	const float r1y{ pos0.y};

	const float l2x{ pos1.x };
	const float l2y{ pos1.y + m_CellSize };
	const float r2x{ pos1.x + m_CellSize };
	const float r2y{ pos1.y };

	constexpr float distanceEpsilon{ 5.0f };

	// if rectangle has area 0, no overlap
	if (l1x == r1x || l1y == r1y || r2x == l2x || l2y == r2y)
		return false;

	// If one rectangle is on left side of other
	if (l1x >= r2x - distanceEpsilon || l2x >= r1x - distanceEpsilon)
		return false;	

	// If one rectangle is above other
	if (r1y >= l2y - distanceEpsilon || r2y >= l1y - distanceEpsilon)
		return false;

	return true;
}
