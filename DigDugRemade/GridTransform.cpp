#include "GridTransform.h"
#include "GridComponent.h"

#include "GameObject.h"
#include "Transform.h"
#include "TextureRenderer.h"

#include "Timer.h"

void digdug::GridTransform::Init()
{
	m_pTexture = GetOwner()->GetComponent<that::TextureRenderer>();

	m_pGrid = GetOwner()->GetParent()->GetComponent<GridComponent>();
	const float cellSize{ m_pGrid->GetCellSize() };
	m_pTexture->SetScale(cellSize / m_pTexture->GetTextureSize().x);

	const int stepsPerCell{ m_pGrid->GetStepsPerCell() };

	const auto& position{ GetTransform()->GetLocalPosition() };

	// Set the position in pixels
	m_Position.x = static_cast<int>(position.x / cellSize) * stepsPerCell;
	m_Position.y = static_cast<int>(position.y / cellSize) * stepsPerCell;

	// Set the position in floating point values
	m_FloatPosition.x = static_cast<float>(m_Position.x);
	m_FloatPosition.y = static_cast<float>(m_Position.y);
}

void digdug::GridTransform::Update()
{
	const int stepsPerCell{ m_pGrid->GetStepsPerCell() };
	const float cellSize{ m_pGrid->GetCellSize() };

	// Calculate local position in grid
	const glm::vec2 gridPosition
	{
		m_FloatPosition.x / stepsPerCell * cellSize,
		m_FloatPosition.y / stepsPerCell * cellSize
	};

	GetTransform()->SetLocalPosition(gridPosition);
}

void digdug::GridTransform::Move(int xSteps, int ySteps)
{
	if (!m_Enabled) return;

	const float moveSpeed{ m_pGrid->GetCellSize() };

	glm::vec2 prevPos{ m_FloatPosition };

	if (abs(ySteps) > 0 && m_Position.x % static_cast<int>(m_pGrid->GetStepsPerCell()) == 0)
	{
		// There is Y input and has space on grid to move vertically, move the transform on the Y axis
		m_FloatPosition.y += ySteps * moveSpeed * that::Timer::GetInstance().GetElapsed();
		m_PrevX = 0;
		m_PrevY = ySteps;
	}
	else if (abs(xSteps) > 0 && m_Position.y % static_cast<int>(m_pGrid->GetStepsPerCell()) == 0)
	{
		// There is X input and has space on grid to move horizontally, move the transform on the X axis
		m_FloatPosition.x += xSteps * moveSpeed * that::Timer::GetInstance().GetElapsed();
		m_PrevY = 0;
		m_PrevX = xSteps;
	}
	else if (abs(ySteps) > 0)
	{
		// There is Y input but there is no space on the grid, move the transform on the X axis
		m_FloatPosition.x += m_PrevX * moveSpeed * that::Timer::GetInstance().GetElapsed();
	}
	else
	{
		// There is X input but there is no space on the grid, move the transform on the Y axis
		m_FloatPosition.y += m_PrevY * moveSpeed * that::Timer::GetInstance().GetElapsed();
	}

	// Rotate the transform depending on the direction
	constexpr float rightAngle{ 90.0f };
	if (abs(m_PrevX))
	{
		GetTransform()->SetLocalRotation(rightAngle - m_PrevX * rightAngle);

		glm::vec2 scale{ m_pTexture->GetScale() };
		scale.y = abs(scale.y) * m_PrevX;
		m_pTexture->SetScale(scale);
	}
	else if (abs(m_PrevY))
	{
		GetTransform()->SetLocalRotation(m_PrevY * rightAngle);

		glm::vec2 scale{ m_pTexture->GetScale() };
		scale.y = -abs(scale.y) * m_PrevY;
		m_pTexture->SetScale(scale);
	}

	// Update the pixel position
	m_Position.x = static_cast<int>(m_FloatPosition.x);
	m_Position.y = static_cast<int>(m_FloatPosition.y);

	if (!m_pGrid->IsValidPosition(m_Position))
	{
		m_FloatPosition = prevPos;

		// Update the pixel position
		m_Position.x = static_cast<int>(m_FloatPosition.x);
		m_Position.y = static_cast<int>(m_FloatPosition.y);
	}
}

void digdug::GridTransform::SetPosition(int x, int y)
{
	const int steps{ m_pGrid->GetStepsPerCell() };

	m_Position.x = x * steps;
	m_Position.y = y * steps;

	m_FloatPosition.x = static_cast<float>(x * steps);
	m_FloatPosition.y = static_cast<float>(y * steps);
}

glm::ivec2 digdug::GridTransform::GetPosition() const
{
	return m_Position / m_pGrid->GetStepsPerCell();
}
