#pragma once

#include "Component.h"

#include <unordered_map>
#include "glm/vec2.hpp"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class GridComponent final : public that::Component
	{
	public:
		GridComponent() = default;
		virtual ~GridComponent() = default;

		GridComponent(const GridComponent& other) = delete;
		GridComponent(GridComponent&& other) = delete;
		GridComponent& operator=(const GridComponent& other) = delete;
		GridComponent& operator=(GridComponent&& other) = delete;

		virtual void Update() override;
		virtual void Render() const override;

		void SetStepsPerCell(int steps) { m_StepsPerCell = steps; }
		void SetCellSize(float cellSize) { m_CellSize = cellSize; }

		int GetStepsPerCell() const { return m_StepsPerCell; }
		const glm::vec2& GetPivot() const;
		float GetCellSize() const { return m_CellSize; }
	private:
		float m_CellSize{ 16 };
		int m_StepsPerCell{ 16 };
		int m_GridSize{ 12 };
	};
}

