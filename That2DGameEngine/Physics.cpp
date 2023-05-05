#include "Physics.h"

#include "BoxCollider.h"
#include "Transform.h"

void that::Physics::AddCollider(BoxCollider* pCollider)
{
	m_pColliders.push_back(pCollider);
}

void that::Physics::RemoveCollider(BoxCollider* pCollider)
{
	m_pColliders.erase(std::remove(begin(m_pColliders), end(m_pColliders), pCollider));
}

void that::Physics::Update()
{
	// Loop over all known colliders
	for (auto pChild : m_pColliders)
	{
		for (auto pOther : m_pColliders)
		{
			// Don't try collision with itself
			if (pChild == pOther) continue;

			if (DoOverlap(pChild, pOther)) pChild->Hit(pOther);
		}
	}
}

bool that::Physics::DoOverlap(BoxCollider* pCollider, BoxCollider* pOther) const
{
	const auto& colPos{ pCollider->GetTransform()->GetWorldPosition() };
	const auto& colSize{ pCollider->GetSize() };

	const auto& otherPos{ pOther->GetTransform()->GetWorldPosition() };
	const auto& otherSize{ pOther->GetSize() };

	const float l1x{ colPos.x };
	const float l1y{ colPos.y + colSize.y };
	const float r1x{ colPos.x + colSize.x };
	const float r1y{ colPos.y };

	const float l2x{ otherPos.x };
	const float l2y{ otherPos.y + otherSize.y };
	const float r2x{ otherPos.x + otherSize.x };
	const float r2y{ otherPos.y };

	constexpr float distanceEpsilon{ 0.01f };

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