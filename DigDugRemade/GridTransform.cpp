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
	const glm::vec2 gridPosition{ m_FloatPosition / static_cast<float>(stepsPerCell) * cellSize };

	GetTransform()->SetLocalPosition(gridPosition);
}

bool digdug::GridTransform::Move(int xSteps, int ySteps, bool checkWorld)
{
	if (!m_Enabled) return false;

	const float moveSpeed{ m_pGrid->GetCellSize() };

	glm::vec2 prevPos{ m_FloatPosition };

	const int posInTileX{ m_Position.x % static_cast<int>(m_pGrid->GetStepsPerCell()) };
	const int posInTileY{ m_Position.y % static_cast<int>(m_pGrid->GetStepsPerCell()) };
	if (abs(ySteps) > 0 && (checkWorld && (posInTileX <= 3 || posInTileX >= m_pGrid->GetStepsPerCell() - 3) || !checkWorld && posInTileX == 0))
	{
		m_FloatPosition.x = static_cast<float>(m_Position.x / static_cast<int>(m_pGrid->GetStepsPerCell()) * static_cast<int>(m_pGrid->GetStepsPerCell()));
		// There is Y input and has space on grid to move vertically, move the transform on the Y axis
		m_FloatPosition.y += ySteps * moveSpeed * that::Timer::GetInstance().GetElapsed();
		m_PrevX = 0;
		m_PrevY = ySteps;
	}
	else if (abs(xSteps) > 0 && (checkWorld && (posInTileY <= 3 || posInTileY >= m_pGrid->GetStepsPerCell() - 3) || !checkWorld && posInTileY == 0))
	{
		m_FloatPosition.y = static_cast<float>(m_Position.y / static_cast<int>(m_pGrid->GetStepsPerCell()) * static_cast<int>(m_pGrid->GetStepsPerCell()));
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
	
	const glm::ivec2 prevPosition{ m_Position };

	m_FloatPosition.x = std::clamp(m_FloatPosition.x, 0.0f, static_cast<float>((m_pGrid->GetSize() - 1) * m_pGrid->GetStepsPerCell()));
	m_FloatPosition.y = std::clamp(m_FloatPosition.y, 0.0f, static_cast<float>((m_pGrid->GetSize() - 1) * m_pGrid->GetStepsPerCell()));

	// Update the pixel position
	m_Position.x = static_cast<int>(m_FloatPosition.x);
	m_Position.y = static_cast<int>(m_FloatPosition.y);

	bool revertedMovement{ false };

	while (!m_pGrid->IsValidPosition(m_Position, { m_PrevX,m_PrevY }, checkWorld) && 
		(m_Position.x != prevPosition.x || m_Position.y != prevPosition.y))
	{
		revertedMovement = true;

		if (const int difX{ m_Position.x - prevPosition.x }; abs(difX) > 3)
		{
			m_Position = prevPos;
			break;
		}
		if (const int difY{ m_Position.y - prevPosition.y }; abs(difY) > 3)
		{
			m_Position = prevPos;
			break;
		}

		m_Position.x -= m_PrevX;
		m_Position.y -= m_PrevY;

		if (m_Position.x < 0)
		{
			m_Position.x = 0;
			break;
		}
		if(m_Position.y < 0)
		{
			m_Position.y = 0;
			break;
		}
	}

	if (m_Position.x == prevPosition.x && m_Position.y == prevPosition.y && revertedMovement)
	{
		m_FloatPosition = prevPos;

		// Reset the pixel position
		m_Position.x = static_cast<int>(m_FloatPosition.x);
		m_Position.y = static_cast<int>(m_FloatPosition.y);

		return false;
	}
	else if (revertedMovement)
	{
		// Correct the float position
		m_FloatPosition.x = m_Position.x + m_FloatPosition.x - static_cast<int>(m_FloatPosition.x);
		m_FloatPosition.y = m_Position.y + m_FloatPosition.y - static_cast<int>(m_FloatPosition.y);
	}
	else if(abs(m_FloatPosition.x - prevPos.x) < FLT_EPSILON && abs(m_FloatPosition.y - prevPos.y) < FLT_EPSILON)
	{
		return false;
	}

	m_MoveEvent.Notify(*this);

	if (const float difX{ m_FloatPosition.x - prevPos.x }; abs(difX) > 0.0f)
	{
		m_PrevX = static_cast<int>(difX / abs(difX));
	}
	else
	{
		m_PrevX = 0;
	}

	if (const float difY{ m_FloatPosition.y - prevPos.y }; abs(difY) > 0.0f)
	{
		m_PrevY = static_cast<int>(difY / abs(difY));
	}
	else
	{
		m_PrevY = 0;
	}

	return true;
}

void digdug::GridTransform::SetPosition(int x, int y)
{
	const int steps{ m_pGrid->GetStepsPerCell() };

	m_Position.x = x * steps;
	m_Position.y = y * steps;

	m_FloatPosition.x = static_cast<float>(x * steps);
	m_FloatPosition.y = static_cast<float>(y * steps);
}

glm::ivec2 digdug::GridTransform::GetCellPosition() const
{
	return m_Position / m_pGrid->GetStepsPerCell();
}
