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

	const int maxGridIdxX{ m_GridSize.x -1 };
	const int maxGridIdxY{ m_GridSize.y - 1 };
	if (position.x > maxGridIdxX * m_CellSize || position.y > maxGridIdxY * m_CellSize)
		return false;

	if (!checkWorld) return true;

	for (const auto& tilePair : m_pTiles)
	{
		WorldTile* pTile{ tilePair.second };

		if (!pTile) continue;

		if (!pTile->IsValidPosition(position, direction, m_CellSize))
			return false;
	}

	return true;
}

bool digdug::GridComponent::IsOpenPosition(const glm::ivec2& position) const
{
	if (position.x < 0.0f || position.y < 0.0f)
		return false;

	const int maxGridIdxX{ m_GridSize.x - 1 };
	const int maxGridIdxY{ m_GridSize.y - 1 };
	if (position.x > maxGridIdxX * m_CellSize || position.y > maxGridIdxY * m_CellSize)
		return false;

	const glm::ivec2 gridPos{ position / static_cast<int>(m_CellSize) };

	for (const auto& tilePair : m_pTiles)
	{
		WorldTile* pTile{ tilePair.second };

		if (!pTile) continue;

		if (pTile->GetGridPosition() != gridPos) continue;

		return pTile->IsOpen();
	}

	return false;
}

digdug::WorldTile* digdug::GridComponent::GetTile(int x, int y) const
{
	return std::find_if(begin(m_pTiles), end(m_pTiles), [x, y](const auto& tilePair) { return tilePair.first.x == x && tilePair.first.y == y; })->second;
}

digdug::WorldTile* digdug::GridComponent::GetOpenTile() const
{
	std::vector<WorldTile*> pOpenTiles{};

	for (const auto& tilePair : m_pTiles)
	{
		WorldTile* pTile{ tilePair.second };
		if (!pTile) continue;

		if (pTile->IsOpen()) pOpenTiles.push_back(pTile);
	}

	return pOpenTiles[rand() % pOpenTiles.size()];
}

void digdug::GridComponent::SetSize(int x, int y)
{
	m_GridSize = { x,y };
	if (m_pTiles.empty()) m_pTiles.resize(x * y);
}

void digdug::GridComponent::SetTile(int x, int y, WorldTile* pWorldTile)
{
	m_pTiles[y * m_GridSize.x + x] = std::make_pair(glm::ivec2{x, y}, pWorldTile);
}

void digdug::GridComponent::BindPlayer(GridTransform* pPlayer)
{
	pPlayer->OnMove.AddListener(this);
	m_pPlayers.push_back(pPlayer);
}

void digdug::GridComponent::Notify(const GridTransform& transform)
{
	for (const auto& tilePair : m_pTiles)
	{
		WorldTile* pTile{ tilePair.second };

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
