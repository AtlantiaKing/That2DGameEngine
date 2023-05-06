#include "GridComponent.h"

#include "GameObject.h"
#include "Transform.h"
#include "WorldTile.h"
#include "GridTransform.h"

#include "Renderer.h"
#include "ResourceManager.h"

const glm::vec2& digdug::GridComponent::GetPivot() const
{
	return GetTransform()->GetWorldPosition();
}

bool digdug::GridComponent::IsValidPosition(const glm::vec2& position, const glm::ivec2& direction, bool checkWorld)
{
	const glm::vec2 gridPos{ position / static_cast<float>(m_StepsPerCell) * m_CellSize };

	if (position.x < 0.0f || position.y < 0.0f) return false;

	const int maxGridIdx{ m_GridSize - 1 };
	if (position.x > maxGridIdx * m_CellSize || position.y > maxGridIdx * m_CellSize) return false;

	if (!checkWorld) return true;

	bool validPos{ true };

	for (WorldTile* pTile : m_pTiles)
	{
		if (pTile) validPos &= pTile->IsValidPosition(gridPos, direction, m_CellSize);

		if (!validPos) break;
	}

	return validPos;
}

void digdug::GridComponent::SetTile(int x, int y, WorldTile* pWorldTile)
{
	if (m_pTiles.empty()) m_pTiles.resize(m_GridSize * m_GridSize);

	m_pTiles[y * m_GridSize + x] = pWorldTile;
}

void digdug::GridComponent::BindPlayer(GridTransform* pPlayer)
{
	pPlayer->OnMove().AddListener(this);
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
