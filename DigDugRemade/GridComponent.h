#pragma once

#include "Component.h"

#include "Observer.h"

#include <unordered_map>
#include "glm/vec2.hpp"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class WorldTile;
	class GridTransform;

	class GridComponent final : public that::Component, that::Observer<GridTransform>
	{
	public:
		GridComponent() = default;
		virtual ~GridComponent();

		GridComponent(const GridComponent& other) = delete;
		GridComponent(GridComponent&& other) = delete;
		GridComponent& operator=(const GridComponent& other) = delete;
		GridComponent& operator=(GridComponent&& other) = delete;

		void SetStepsPerCell(int steps) { m_StepsPerCell = steps; }
		void SetCellSize(float cellSize) { m_CellSize = cellSize; }

		int GetStepsPerCell() const { return m_StepsPerCell; }
		const glm::vec2& GetPivot() const;
		float GetCellSize() const { return m_CellSize; }
		bool IsValidPosition(const glm::vec2& position, const glm::ivec2& direction, bool checkWorld = false);
		bool IsOpenPosition(const glm::ivec2 position);
		int GetSize() const { return m_GridSize; }

		void SetTile(int x, int y, WorldTile* pWorldTile);
		void BindPlayer(GridTransform* pPlayer);

		virtual void Notify(const GridTransform& change) override;
	private:
		std::vector<WorldTile*> m_pTiles{};
		std::vector<GridTransform*> m_pPlayers{};

		float m_CellSize{ 16 };
		int m_StepsPerCell{ 16 };
		int m_GridSize{ 12 };
	};
}

