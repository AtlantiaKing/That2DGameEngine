#include "GridTransform.h"
#include "GridComponent.h"

#include "GameObject.h"
#include "Transform.h"
#include "TextureComponent.h"

#include "Timer.h"

void digdug::GridTransform::Init()
{
	SnapToGrid();
}

void digdug::GridTransform::Update()
{
	const int stepsPerCell{ m_pGrid->GetStepsPerCell() };
	const float cellSize{ m_pGrid->GetCellSize() };

	// Calculate local position in grid
	const glm::vec2 gridPosition{ m_FloatPosition / static_cast<float>(stepsPerCell) * cellSize };

	GetTransform()->SetLocalPosition(gridPosition);

	if (!m_IsMoving && m_WasMoving) 
		OnStopMove.Notify(*this);
	m_WasMoving = m_IsMoving;
	m_IsMoving = false;
}

void digdug::GridTransform::OnDisable()
{
	OnStopMove.Notify(*this);
	m_WasMoving = false;
	m_IsMoving = false;
}

void digdug::GridTransform::SnapToGrid()
{
	if(!m_pTexture) m_pTexture = GetOwner()->GetComponent<that::TextureComponent>();
	if(!m_pGrid) m_pGrid = GetOwner()->GetParent()->GetComponent<GridComponent>();

	const float cellSize{ m_pGrid->GetCellSize() };

	const int stepsPerCell{ m_pGrid->GetStepsPerCell() };

	const auto& position{ GetTransform()->GetLocalPosition() };

	// Set the position in pixels
	m_Position.x = static_cast<int>((position.x + stepsPerCell / 2.0f) / cellSize) * stepsPerCell;
	m_Position.y = static_cast<int>((position.y + stepsPerCell / 2.0f) / cellSize) * stepsPerCell;

	// Set the position in floating point values
	m_FloatPosition.x = static_cast<float>(m_Position.x);
	m_FloatPosition.y = static_cast<float>(m_Position.y);
}

bool digdug::GridTransform::Move(int xSteps, int ySteps, bool checkWorld)
{
	if (!m_Enabled) return false;

	const float moveSpeed{ m_pGrid->GetCellSize() * GetTransform()->GetWorldScale().x };

	glm::vec2 prevPos{ m_FloatPosition };

	const int cellSteps{ m_pGrid->GetStepsPerCell() };
	const int posInTileX{ m_Position.x % static_cast<int>(cellSteps) };
	const int posInTileY{ m_Position.y % static_cast<int>(cellSteps) };

	const bool canMoveVertical{ checkWorld ? posInTileX < m_IsInTileEpsilon || posInTileX > cellSteps - (m_IsInTileEpsilon+1) : posInTileX == 0};
	const bool canMoveHorizontal{ checkWorld ? posInTileY < m_IsInTileEpsilon || posInTileY > cellSteps - (m_IsInTileEpsilon+1) : posInTileY == 0};

	if (abs(ySteps) > 0 && canMoveVertical)
	{
		if (posInTileX < m_IsInTileEpsilon)
			m_FloatPosition.x = static_cast<float>(m_Position.x - posInTileX);
		else
			m_FloatPosition.x = static_cast<float>(m_Position.x + cellSteps - posInTileX);

		m_FloatPosition.y += ySteps * moveSpeed * that::Timer::GetInstance().GetElapsed();
		m_PrevX = 0;
		m_PrevY = ySteps;
	}
	else if (abs(xSteps) > 0 && canMoveHorizontal)
	{
		if (posInTileY < m_IsInTileEpsilon)
			m_FloatPosition.y = static_cast<float>(m_Position.y - posInTileY);
		else
			m_FloatPosition.y = static_cast<float>(m_Position.y + cellSteps - posInTileY);

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

		glm::vec2 scale{ GetTransform()->GetLocalScale() };
		scale.y = abs(scale.y) * m_PrevX;
		GetTransform()->SetLocalScale(scale);
	}
	else if (abs(m_PrevY))
	{
		if (m_RotateIfUp)
		{
			GetTransform()->SetLocalRotation(m_PrevY * rightAngle);

			glm::vec2 scale{ GetTransform()->GetLocalScale() };
			scale.y = -abs(scale.y) * m_PrevY;
			GetTransform()->SetLocalScale(scale);
		}
		else
		{
			GetTransform()->SetLocalRotation(0.0f);
			GetTransform()->SetLocalScale(1.0f);
		}
	}
	
	const glm::ivec2 prevPosition{ m_Position };

	const auto& gridSize{ m_pGrid->GetSize() };
	m_FloatPosition.x = std::clamp(m_FloatPosition.x, 0.0f, static_cast<float>((gridSize.x - 1) * m_pGrid->GetStepsPerCell()));
	m_FloatPosition.y = std::clamp(m_FloatPosition.y, 0.0f, static_cast<float>((gridSize.y - 1) * m_pGrid->GetStepsPerCell()));

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

	m_IsMoving = true;
	if (!m_WasMoving)
	{
		OnStartMove.Notify(*this);
	}
	OnMove.Notify(*this);

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

bool digdug::GridTransform::CanMoveInDirection(const glm::ivec2& direction) const
{
	const int cellSteps{ m_pGrid->GetStepsPerCell() };
	const int posInTileX{ m_Position.x % static_cast<int>(cellSteps) };
	const int posInTileY{ m_Position.y % static_cast<int>(cellSteps) };

	const bool canMoveVertical{ posInTileX == 0 };
	const bool canMoveHorizontal{ posInTileY == 0 };

	if (direction.x && !canMoveHorizontal) return false;
	if (direction.y && !canMoveVertical) return false;

	if (canMoveHorizontal && canMoveVertical)
	{
		return m_pGrid->IsValidPosition(m_Position + direction, direction, true);
	}

	return false;
}

glm::ivec2 digdug::GridTransform::GetCellPosition() const
{
	return m_Position / m_pGrid->GetStepsPerCell();
}
