#include "FygarRoamingAI.h"

#include "GameObject.h"

#include "GridTransform.h"

#include "Timer.h"

#include "FygarAttackState.h"
#include "FygarGhostState.h"

digdug::FygarRoamingAI::FygarRoamingAI(that::GameObject* pFygar)
	: m_pFygarObj{ pFygar }
{
	m_RoamTime = 0.0f;
	m_TimeUntilStateChange = static_cast<float>(rand()) / RAND_MAX * (m_MaxTimeUntilState - m_MinTimeUntilState) + m_MinTimeUntilState;
}

std::unique_ptr<digdug::EnemyState> digdug::FygarRoamingAI::Update()
{
	m_RoamTime += that::Timer::GetInstance().GetElapsed();
	if (m_RoamTime > m_TimeUntilStateChange)
	{
		if (static_cast<float>(rand()) / RAND_MAX < m_AttackChance)
		{
			return std::make_unique<FygarAttackState>(m_pFygarObj);
		}
		else
		{
			return std::make_unique<FygarGhostState>(m_pFygarObj);
		}
	}

	UpdateMovement();

	return nullptr;
}

void digdug::FygarRoamingAI::UpdateMovement()
{
	auto pTransform{ m_pFygarObj->GetComponent<GridTransform>() };

	// Try to move
	if (pTransform->Move(m_Direction.x, m_Direction.y, true)) return;

	// Change direction if moving fails
	if (rand() % 1001 / 1000.0f > 0.3f)
	{
		m_Direction.x = rand() % 1001 / 1000.0f > 0.5f ? -1 : 1;
		m_Direction.y = 0;
	}
	else
	{
		m_Direction.y = rand() % 1001 / 1000.0f > 0.5f ? -1 : 1;
		m_Direction.x = 0;
	}
}
