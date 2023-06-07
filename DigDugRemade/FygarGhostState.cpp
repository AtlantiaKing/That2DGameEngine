#include "FygarGhostState.h"

#include "GameObject.h"

#include "TextureRenderer.h"
#include "GridComponent.h"
#include "WorldTile.h"
#include "Transform.h"

#include "FygarRoamingState.h"

#include "TextureManager.h"
#include "Timer.h"

digdug::FygarGhostState::FygarGhostState(that::GameObject* pFygar, that::GameObject* pPlayer)
    : m_pFygarObj{ pFygar }
    , m_pPlayer{ pPlayer }
{
}

std::unique_ptr<digdug::FygarState> digdug::FygarGhostState::Update()
{
    that::Transform* pTransform{ m_pFygarObj->GetTransform() };
    const auto& pos{ pTransform->GetLocalPosition() };
    const auto& playerPos{ m_pPlayer->GetTransform()->GetLocalPosition() };

    const glm::vec2 direction{ playerPos - pos };

    const float elapsedSec{ that::Timer::GetInstance().GetElapsed() };

    pTransform->Translate(glm::normalize(direction) * m_MoveSpeed * elapsedSec);

    if (m_WaitForCheck < m_WaitTime) m_WaitForCheck += elapsedSec;
    else
    {
        const glm::ivec2 posInt{ static_cast<int>(pos.x), static_cast<int>(pos.y) };
        const auto stepPos{ posInt % m_pGrid->GetStepsPerCell() };

        if (stepPos.x >= m_pGrid->GetStepsPerCell() - (m_GridEpsilon + 1) || stepPos.x <= m_GridEpsilon ||
            stepPos.y >= m_pGrid->GetStepsPerCell() - (m_GridEpsilon + 1) || stepPos.y <= m_GridEpsilon)
        {
            if (m_pGrid->IsOpenPosition({ static_cast<int>(pos.x), static_cast<int>(pos.y) })) return std::make_unique<FygarRoamingState>(m_pFygarObj, m_pPlayer);
        }
    }

    return nullptr;
}

void digdug::FygarGhostState::StateEnter()
{
    const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("Fygar/Ghost.png") };
    m_pFygarObj->GetComponent<that::TextureRenderer>()->SetTexture(pTexture);

    m_pGrid = m_pFygarObj->GetParent()->GetComponent<GridComponent>();
    m_GoToTile = m_pGrid->GetOpenTile()->GetGridPosition() * static_cast<int>(m_pGrid->GetCellSize());
}

void digdug::FygarGhostState::StateEnd()
{
}
