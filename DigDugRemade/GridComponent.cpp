#include "GridComponent.h"

#include "Transform.h"

#include "SDL.h"
#include "Renderer.h"

void digdug::GridComponent::Update()
{
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
