#pragma once

#include "Component.h"

#include "glm/vec2.hpp"

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

		void Move(int xSteps, int ySteps);
		void SetPosition(int x, int y);

		glm::ivec2 GetPosition() const;
	private:
		glm::vec2 m_FloatPosition{};
		glm::ivec2 m_Position{};
		int m_PrevX{ 1 };
		int m_PrevY{ 0 };

		GridComponent* m_pGrid{};
		that::TextureRenderer* m_pTexture{};
	};
}
