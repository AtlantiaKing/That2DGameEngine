#include "PookaRoamingState.h"

#include "GameObject.h"

#include "TextureRenderer.h"
#include "GridTransform.h"

#include "PookaGhostState.h"

#include "ResourceManager.h"
#include "Timer.h"

digdug::PookaRoamingState::PookaRoamingState(that::GameObject* pPooka, that::GameObject* pPlayer)
	: m_pPookaObj{ pPooka }
	, m_pPlayer{ pPlayer }
{
}


std::unique_ptr<digdug::PookaState> digdug::PookaRoamingState::Update()
{
	m_RoamTime += that::Timer::GetInstance().GetElapsed();
	if (m_RoamTime > m_TimeUntilGhost) return std::make_unique<PookaGhostState>(m_pPookaObj, m_pPlayer);

	UpdateMovement();

	return nullptr;
}

void digdug::PookaRoamingState::StateEnter()
{
	GridTransform* pGridTransform{ m_pPookaObj->GetComponent<GridTransform>() };
	pGridTransform->SetEnabled(true);
	pGridTransform->SnapToGrid();

	const auto& pTexture{ that::ResourceManager::GetInstance().LoadTexture("Pooka/Default.png") };
	m_pPookaObj->GetComponent<that::TextureRenderer>()->SetTexture(pTexture);

	m_RoamTime = 0.0f;
	m_TimeUntilGhost = static_cast<float>(rand()) / RAND_MAX * (m_MaxTimeUntilGhost - m_MinTimeUntilGhost) + m_MinTimeUntilGhost;
}

void digdug::PookaRoamingState::StateEnd()
{
	m_pPookaObj->GetComponent<GridTransform>()->SetEnabled(false);
}

void digdug::PookaRoamingState::UpdateMovement()
{
	auto pTransform{ m_pPookaObj->GetComponent<GridTransform>() };

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
