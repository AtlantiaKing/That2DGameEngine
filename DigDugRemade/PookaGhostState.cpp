#include "PookaGhostState.h"

#include "GameObject.h"

#include "SpriteRenderer.h"
#include "GridComponent.h"
#include "WorldTile.h"
#include "Transform.h"

#include "PookaRoamingState.h"

#include "TextureManager.h"
#include "Timer.h"

digdug::PookaGhostState::PookaGhostState(that::GameObject* pPooka, that::GameObject* pPlayer)
    : m_pPookaObj{ pPooka }
    , m_pPlayer{ pPlayer }
{
}

std::unique_ptr<digdug::State> digdug::PookaGhostState::Update()
{
    that::Transform* pTransform{ m_pPookaObj->GetTransform() };
    const auto& pos{ pTransform->GetLocalPosition() };
    const auto& playerPos{ m_pPlayer->GetTransform()->GetLocalPosition() };

    const glm::vec2 direction{ playerPos - pos };

    const float elapsedSec{ that::Timer::GetInstance().GetElapsed() };

    pTransform->Translate(glm::normalize(direction) * m_MoveSpeed * elapsedSec);

    if(m_WaitForCheck < m_WaitTime) m_WaitForCheck += elapsedSec;
    else
    {
        const glm::ivec2 posInt{ static_cast<int>(pos.x), static_cast<int>(pos.y) };
        const int steps{ m_pGrid->GetStepsPerCell() };

        const bool isInTileY{ abs(((posInt.y + steps / 2) % steps) - (steps / 2)) < m_GridEpsilon };
        const bool isInTileX{ abs(((posInt.x + steps / 2) % steps) - (steps / 2)) < m_GridEpsilon };

        if (isInTileX && isInTileY)
        {
            if (m_pGrid->IsOpenPosition({ posInt.x + steps / 2, posInt.y + steps / 2 }))
            {
                return std::make_unique<PookaRoamingState>(m_pPookaObj, m_pPlayer);
            }
        }
    }

    return nullptr;
}

void digdug::PookaGhostState::StateEnter()
{
    const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("Pooka/Ghost.png") };
    m_pPookaObj->GetComponent<that::SpriteRenderer>()->SetSprite(pTexture, 2, 1, 0.4f);

    m_pGrid = m_pPookaObj->GetParent()->GetComponent<GridComponent>();
}

void digdug::PookaGhostState::StateEnd()
{
}
