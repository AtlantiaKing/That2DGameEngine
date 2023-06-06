#include "GridComponent.h"

#include "GameObject.h"
#include "Transform.h"
#include "WorldTile.h"
#include "GridTransform.h"

#include "Renderer.h"
#include "ResourceManager.h"

digdug::GridComponent::~GridComponent()
{
	for (GridTransform* pPlayer : m_pPlayers)
	{
		pPlayer->OnMove.RemoveListener(this);
	}
}

const glm::vec2& digdug::GridComponent::GetPivot() const
{
	return GetTransform()->GetWorldPosition();
}

bool digdug::GridComponent::IsValidPosition(const glm::vec2& position, const glm::ivec2& direction, bool checkWorld) const
{
	if (position.x < 0.0f || position.y < 0.0f) 
		return false;

	const int maxGridIdx{ m_GridSize - 1 };
	if (position.x > maxGridIdx * m_CellSize || position.y > maxGridIdx * m_CellSize) 
		return false;

	if (!checkWorld) return true;

	const glm::vec2 gridPos{ position / static_cast<float>(m_StepsPerCell) * m_CellSize };

	for (WorldTile* pTile : m_pTiles)
	{
		if (!pTile) continue;

		if (!pTile->IsValidPosition(gridPos, direction, m_CellSize)) 
			return false;
	}

	return true;
}

bool digdug::GridComponent::IsOpenPosition(const glm::ivec2& position) const
{
	if (position.x < 0.0f || position.y < 0.0f)
		return false;

	const int maxGridIdx{ m_GridSize - 1 };
	if (position.x > maxGridIdx * m_CellSize || position.y > maxGridIdx * m_CellSize)
		return false;

	const glm::ivec2 gridPos{ position / static_cast<int>(m_CellSize) };

	for (WorldTile* pTile : m_pTiles)
	{
		if (!pTile) continue;

		if (pTile->GetGridPosition() != gridPos) continue;

		return pTile->IsOpen();
	}

	return false;
}

void digdug::GridComponent::SetTile(int x, int y, WorldTile* pWorldTile)
{
	if (m_pTiles.empty()) m_pTiles.resize(m_GridSize * m_GridSize);

	m_pTiles[y * m_GridSize + x] = pWorldTile;
}

void digdug::GridComponent::BindPlayer(GridTransform* pPlayer)
{
	pPlayer->OnMove.AddListener(this);
	m_pPlayers.push_back(pPlayer);
}

void digdug::GridComponent::Notify(const GridTransform& transform)
{
	for (WorldTile* pTile : m_pTiles)
	{
		if (!pTile) continue;

		const auto intPos{ transform.GetPosition() };
		const glm::vec2 gridPos
		{ 
			intPos.x / static_cast<float>(m_StepsPerCell) * m_CellSize,
			intPos.y / static_cast<float>(m_StepsPerCell) * m_CellSize
		};
		pTile->UpdatePlayer(transform.GetCellPosition(), gridPos, transform.GetDirection(), m_CellSize);
	}
}
