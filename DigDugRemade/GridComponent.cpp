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
	pColliders.reserve(children.size());

	for (auto pChild : GetOwner()->GetChildren())
	{
		auto pCollider{ pChild->GetComponent<GridCollider>() };
		if (!pCollider) continue;

		pColliders.push_back(pCollider);
	}

	for (auto pChild : pColliders)
	{
		const auto& childPos{ pChild->GetOwner()->GetTransform()->GetLocalPosition() };
		for (auto pOther : pColliders)
		{
			const auto& otherPos{ pOther->GetOwner()->GetTransform()->GetLocalPosition() };

			if (DoOverlap(childPos, otherPos))
			{
				pChild->Hit(pOther);
			}
		}
	}
}

void digdug::GridComponent::Render() const
{
	for (int x{}; x < m_GridSize; ++x)
	{
		for (int y{}; y < m_GridSize; ++y)
		{
			const glm::vec2& pivot{ GetTransform()->GetWorldPosition() };

			SDL_SetRenderDrawColor(that::Renderer::GetInstance().GetSDLRenderer(), UINT8_MAX, UINT8_MAX, UINT8_MAX, UINT8_MAX);
			const SDL_Rect gridCell
			{
				static_cast<int>(pivot.x + x * m_CellSize),
				static_cast<int>(pivot.y + y * m_CellSize),
				static_cast<int>(m_CellSize),
				static_cast<int>(m_CellSize)
			};
			SDL_RenderDrawRect(that::Renderer::GetInstance().GetSDLRenderer(), &gridCell);
		}
	}
}

const glm::vec2& digdug::GridComponent::GetPivot() const
{
	return GetTransform()->GetWorldPosition();
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

	// if rectangle has area 0, no overlap
	if (l1x == r1x || l1y == r1y || r2x == l2x || l2y == r2y)
		return false;

	// If one rectangle is on left side of other
	if (l1x > r2x || l2x > r1x)
		return false;

	// If one rectangle is above other
	if (r1y > l2y || r2y > l1y)
		return false;

	return true;
}
