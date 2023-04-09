#include "GridTransform.h"
#include "GridComponent.h"

#include "GameObject.h"
#include "Transform.h"

#include "Timer.h"

void digdug::GridTransform::Init()
{
	m_pGrid = GetOwner()->GetParent()->GetComponent<GridComponent>();
	const float cellSize{ m_pGrid->GetCellSize() };
	const int stepsPerCell{ m_pGrid->GetStepsPerCell() };

	const auto& worldPos{ GetTransform()->GetWorldPosition() };
	m_Position.x = static_cast<int>(worldPos.x / cellSize) * stepsPerCell;
	m_Position.y = static_cast<int>(worldPos.y / cellSize) * stepsPerCell;
	m_FloatPosition.x = static_cast<float>(m_Position.x);
	m_FloatPosition.y = static_cast<float>(m_Position.y);
}

void digdug::GridTransform::Update()
{
	const int stepsPerCell{ m_pGrid->GetStepsPerCell() };
	const float cellSize{ m_pGrid->GetCellSize() };

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

	if (abs(ySteps) > 0 && m_Position.x % static_cast<int>(m_pGrid->GetStepsPerCell()) == 0)
	{
		m_FloatPosition.y += ySteps * moveSpeed * that::Timer::GetInstance().GetElapsed();
		m_PrevX = 0;
		m_PrevY = ySteps;
	}
	else if(abs(xSteps) == 0)
	{
		m_FloatPosition.x += m_PrevX * moveSpeed * that::Timer::GetInstance().GetElapsed();
	} 
	else if (abs(xSteps) > 0 && m_Position.y % static_cast<int>(m_pGrid->GetStepsPerCell()) == 0)
	{
		m_FloatPosition.x += xSteps * moveSpeed * that::Timer::GetInstance().GetElapsed();
		m_PrevY = 0;
		m_PrevX = xSteps;
	}
	else
	{
		m_FloatPosition.y += m_PrevY * moveSpeed * that::Timer::GetInstance().GetElapsed();
	}

	constexpr float rightAngle{ 90.0f };
	if (abs(m_PrevX)) GetTransform()->SetLocalRotation(rightAngle - m_PrevX * rightAngle);
	if (abs(m_PrevY)) GetTransform()->SetLocalRotation(m_PrevY * rightAngle);

	m_Position.x = static_cast<int>(m_FloatPosition.x);
	m_Position.y = static_cast<int>(m_FloatPosition.y);
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
