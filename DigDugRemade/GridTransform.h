#pragma once

#include "Component.h"

#include "glm/vec2.hpp"

#include "Subject.h"

namespace that
{
	class TextureRenderer;
}

namespace digdug
{
	class GridComponent;

	class GridTransform final : public that::Component
	{
	public:
		GridTransform() = default;
		virtual ~GridTransform() = default;

		GridTransform(const GridTransform& other) = delete;
		GridTransform(GridTransform&& other) = delete;
		GridTransform& operator=(const GridTransform& other) = delete;
		GridTransform& operator=(GridTransform&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;
		virtual void OnDisable() override;

		void ShouldRotateWhenGoingUp(bool rotate) { m_RotateIfUp = rotate; }
		void SnapToGrid();
		bool Move(int xSteps, int ySteps, bool checkWorld = false);
		void SetPosition(int x, int y);

		glm::ivec2 GetCellPosition() const;
		glm::ivec2 GetPosition() const { return m_Position; }
		glm::ivec2 GetDirection() const { return { m_PrevX, m_PrevY }; }

		that::Subject<GridTransform> OnStartMove{};
		that::Subject<GridTransform> OnMove{};
		that::Subject<GridTransform> OnStopMove{};
	private:
		glm::vec2 m_FloatPosition{};
		glm::ivec2 m_Position{};
		int m_PrevX{ 1 };
		int m_PrevY{ 0 };
		bool m_IsMoving{};
		bool m_WasMoving{};
		bool m_RotateIfUp{ false };

		const int m_ChangeDirectionEpsilon{ 3 };

		GridComponent* m_pGrid{};
		that::TextureRenderer* m_pTexture{};
	};
}
