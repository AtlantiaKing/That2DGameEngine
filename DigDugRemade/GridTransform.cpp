#include "GridTransform.h"
#include "GridComponent.h"

#include "GameObject.h"
#include "Transform.h"

#include "Timer.h"

void digdug::GridTransform::Init()
{
	m_pGrid = GetOwner()->GetParent()->GetComponent<GridComponent>();
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

	m_Position.x = static_cast<int>(m_FloatPosition.x);
	m_Position.y = static_cast<int>(m_FloatPosition.y);
}
