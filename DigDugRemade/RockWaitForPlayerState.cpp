#include "RockWaitForPlayerState.h"

#include "GameObject.h"

#include "Transform.h"
#include "GridComponent.h"

#include "glm/vec2.hpp"

#include "RockBreakingFreeState.h"

digdug::RockWaitForPlayerState::RockWaitForPlayerState(that::GameObject* pRock, that::GameObject* pPlayer)
    : m_pRock{ pRock }
    , m_pPlayer{ pPlayer }
{
}

std::unique_ptr<digdug::EnemyState> digdug::RockWaitForPlayerState::Update()
{
    const float cellSize{ m_pGrid->GetCellSize() };
    const glm::vec2 rockGridPos{ m_pRock->GetTransform()->GetLocalPosition() / cellSize };
    const glm::vec2 playerGridPos{ m_pPlayer->GetTransform()->GetLocalPosition() / cellSize };

    if (playerGridPos.x < rockGridPos.x - m_GridEpsilonX || playerGridPos.x > rockGridPos.x + m_GridEpsilonX || playerGridPos.y > rockGridPos.y + m_GridEpsilonY)
    {
        return std::make_unique<RockBreakingFreeState>(m_pRock, m_pPlayer);
    }

    return nullptr;
}

void digdug::RockWaitForPlayerState::StateEnter()
{
    m_pGrid = m_pRock->GetParent()->GetComponent<GridComponent>();
}

void digdug::RockWaitForPlayerState::StateEnd()
{
}
