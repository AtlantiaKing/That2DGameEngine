#include "FygarRoamingState.h"


#include "GameObject.h"

#include "TextureRenderer.h"
#include "GridTransform.h"

#include "FygarGhostState.h"
#include "FygarAttackState.h"

#include "ResourceManager.h"
#include "Timer.h"

digdug::FygarRoamingState::FygarRoamingState(that::GameObject* pFygar, that::GameObject* pPlayer)
	: m_pFygarObj{ pFygar }
	, m_pPlayer{ pPlayer }
{
}

std::unique_ptr<digdug::FygarState> digdug::FygarRoamingState::Update()
{
	m_RoamTime += that::Timer::GetInstance().GetElapsed();
	if (m_RoamTime > m_TimeUntilStateChange)
	{
		if (static_cast<float>(rand()) / RAND_MAX < m_AttackChance)
		{
			return std::make_unique<FygarAttackState>(m_pFygarObj, m_pPlayer);
		}
		else
		{
			return std::make_unique<FygarGhostState>(m_pFygarObj, m_pPlayer);
		}
	}

	UpdateMovement();

	return nullptr;
}

void digdug::FygarRoamingState::StateEnter()
{
	GridTransform* pGridTransform{ m_pFygarObj->GetComponent<GridTransform>() };
	pGridTransform->SnapToGrid();

	const auto& pTexture{ that::ResourceManager::GetInstance().LoadTexture("Fygar/Default.png") };
	m_pFygarObj->GetComponent<that::TextureRenderer>()->SetTexture(pTexture);

	m_RoamTime = 0.0f;
	m_TimeUntilStateChange = static_cast<float>(rand()) / RAND_MAX * (m_MaxTimeUntilState - m_MinTimeUntilState) + m_MinTimeUntilState;
}

void digdug::FygarRoamingState::StateEnd()
{
}

void digdug::FygarRoamingState::UpdateMovement()
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
