#include "Rock.h"

#include "GameObject.h"

#include "GridComponent.h"
#include "Transform.h"

#include "RockStaticState.h"

void digdug::Rock::Init()
{
	GridComponent* pGrid{ GetOwner()->GetParent()->GetComponent<GridComponent>() };
	
	pGrid->DisableTile(GetTransform()->GetLocalPosition() / pGrid->GetCellSize());
}

void digdug::Rock::Update()
{
	std::unique_ptr<EnemyState> pNewState{ m_pState->Update() };

	if (pNewState) ChangeState(std::move(pNewState));
}

void digdug::Rock::Start(const std::vector<that::GameObject*>& pPlayers)
{
	m_pPlayers = pPlayers;
	ChangeState(std::make_unique<RockStaticState>(GetOwner()));
}

that::GameObject* digdug::Rock::GetDiggingPlayer()
{
	GridComponent* pGrid{ GetOwner()->GetParent()->GetComponent<GridComponent>() };
	const float cellSize{ pGrid->GetCellSize() };
	const glm::vec2 rockGridPos{ GetTransform()->GetLocalPosition() / cellSize };

	for (that::GameObject* pPlayer : m_pPlayers)
	{
		const glm::vec2 playerGridPos{ pPlayer->GetTransform()->GetLocalPosition() / cellSize };

		if (playerGridPos.x >= rockGridPos.x - m_GridEpsilonX && playerGridPos.x <= rockGridPos.x + m_GridEpsilonX && playerGridPos.y <= rockGridPos.y + m_GridEpsilonY)
		{
			m_pLastPlayer = pPlayer;
			return pPlayer;
		}
	}

	return nullptr;
}

that::GameObject* digdug::Rock::GetLastPlayer() const
{
	return m_pLastPlayer;
}

void digdug::Rock::ChangeState(std::unique_ptr<digdug::EnemyState> pState)
{
	if (m_pState) m_pState->StateEnd();

	m_pState = std::move(pState);
	m_pState->StateEnter();
}
