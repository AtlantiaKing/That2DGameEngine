#include "Physics.h"

#include "BoxCollider.h"
#include "Transform.h"
#include "GameObject.h"

void that::Physics::AddCollider(BoxCollider* pCollider)
{
	m_pColliders.push_back(pCollider);
}

void that::Physics::RemoveCollider(BoxCollider* pCollider)
{
	if (m_pColliders.empty()) return;

	m_pColliders.erase(std::remove(begin(m_pColliders), end(m_pColliders), pCollider), end(m_pColliders));
}

void that::Physics::ActivateDebugRendering(bool showDebug)
{
	m_ShowDebug = showDebug;
}

bool that::Physics::IsShowingDebugRendering() const
{
	return m_ShowDebug;
}

void that::Physics::SetCollisionMargin(float margin)
{
	m_DistanceEpsilon = margin;
}

void that::Physics::Update()
{
	std::vector<std::pair<BoxCollider*, BoxCollider*>> collisions{};

	// Loop over all known colliders
	for (auto pChild : m_pColliders)
	{
		if (!pChild->IsEnabled() || !pChild->GetOwner()->IsActive()) continue;

		for (auto pOther : m_pColliders)
		{
			if (!pOther->IsEnabled() || !pOther->GetOwner()->IsActive()) continue;

			// Don't try collision with itself
			if (pChild == pOther) continue;

			// Don't try collision if this collision already occured
			const auto& collisionPairIt{ std::find_if(begin(collisions), end(collisions), 
				[pChild,pOther](const auto& collisionPair) 
				{ 
					return (collisionPair.first == pChild || collisionPair.second == pChild) && (collisionPair.first == pOther || collisionPair.second == pOther); 
				}) };
			if (collisionPairIt != end(collisions)) continue;

			// Don't try collision if one collider 
			//	is in the ignore group of the other box collider
			if (pChild->GetIgnoreGroup() & pOther->GetLayer()) continue;
			if (pOther->GetIgnoreGroup() & pChild->GetLayer()) continue;

			// Try collision
			CollisionData collisionData{ pChild, pOther };
			if (DoOverlap(collisionData))
			{
				// Handle a collision hit
				pChild->Hit(collisionData);

				// Invert all the collision data
				collisionData.pCollider = pOther;
				collisionData.pOther = pChild;
				collisionData.fixStep = -collisionData.fixStep;

				// Handle a collision hit in the other
				pOther->Hit(collisionData);

				// Cache this collision
				collisions.emplace_back(std::make_pair(pChild, pOther));
			}
		}
	}
}

bool that::Physics::DoOverlap(CollisionData& collisionData) const
{
	BoxCollider* pCollider{ collisionData.pCollider };
	BoxCollider* pOther{ collisionData.pOther };

	const auto& colPos{ pCollider->GetCenterWorld() };
	const auto& colHalfSize{ pCollider->GetSizeWorld() / 2.0f };

	const auto& otherPos{ pOther->GetCenterWorld() };
	const auto& otherHalfSize{ pOther->GetSizeWorld() / 2.0f };

	const float l1x{ colPos.x - colHalfSize.x };
	const float l1y{ colPos.y + colHalfSize.y };
	const float r1x{ colPos.x + colHalfSize.x };
	const float r1y{ colPos.y - colHalfSize.y };

	const float l2x{ otherPos.x - otherHalfSize.x };
	const float l2y{ otherPos.y + otherHalfSize.y };
	const float r2x{ otherPos.x + otherHalfSize.x };
	const float r2y{ otherPos.y - otherHalfSize.y };

	// If one rectangle has area 0, no overlap
	if (l1x == r1x || l1y == r1y || r2x == l2x || l2y == r2y)
		return false;

	// If one rectangle is on left side of other, no overlap
	if (l1x >= r2x - m_DistanceEpsilon || l2x >= r1x - m_DistanceEpsilon)
		return false;

	// If one rectangle is above other, no overlap
	if (r1y >= l2y - m_DistanceEpsilon || r2y >= l1y - m_DistanceEpsilon)
		return false;

	// Calculate the distance between the two colliders
	const glm::vec2 distance{ colPos - otherPos };

	// Calculate the minimum and maximum distances along each axis
	float xOverlap = colHalfSize.x + otherHalfSize.x - std::abs(distance.x);
	float yOverlap = colHalfSize.y + otherHalfSize.y - std::abs(distance.y);

	// Find the axis with the minimum overlap
	if (xOverlap < yOverlap)
	{
		// The collision occurs on the x-axis

		collisionData.contactPoint.x = (distance.x > 0) ? colPos.x - colHalfSize.x : colPos.x + colHalfSize.x;
		collisionData.contactPoint.y = otherPos.y;

		// The movement that needs to happen to solve this collision
		collisionData.fixStep.x = (distance.x > 0 ? otherPos.x + otherHalfSize.x : otherPos.x - otherHalfSize.x) - collisionData.contactPoint.x;
		collisionData.fixStep.y = 0.0f;
	}
	else
	{
		// The collision occurs on the y-axis

		collisionData.contactPoint.x = otherPos.x;
		collisionData.contactPoint.y = (distance.y > 0) ? colPos.y - colHalfSize.y : colPos.y + colHalfSize.y;

		// The movement that needs to happen to solve this collision
		collisionData.fixStep.x = 0.0f;
		collisionData.fixStep.y = (distance.y > 0 ? otherPos.y + otherHalfSize.y : otherPos.y - otherHalfSize.y) - collisionData.contactPoint.y;
	}

	return true;
}
