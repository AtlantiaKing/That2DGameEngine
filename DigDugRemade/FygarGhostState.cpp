#include "FygarGhostState.h"

#include "GameObject.h"

#include "SpriteRenderer.h"
#include "GridComponent.h"
#include "WorldTile.h"
#include "Transform.h"
#include "Fygar.h"

#include "FygarRoamingState.h"

#include "TextureManager.h"
#include "Timer.h"

digdug::FygarGhostState::FygarGhostState(that::GameObject* pFygar)
    : m_pFygarObj{ pFygar }
{
}

std::unique_ptr<digdug::EnemyState> digdug::FygarGhostState::Update()
{
    that::Transform* pTransform{ m_pFygarObj->GetTransform() };
    const auto& pos{ pTransform->GetLocalPosition() };
    const auto& playerPos{ m_pFygar->GetPlayer()->GetTransform()->GetLocalPosition()};

    const glm::vec2 direction{ playerPos - pos };

    const float elapsedSec{ that::Timer::GetInstance().GetElapsed() };

    pTransform->Translate(glm::normalize(direction) * m_MoveSpeed * elapsedSec);
    
    if (m_WaitForCheck < m_WaitTime) m_WaitForCheck += elapsedSec;
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
                return std::make_unique<FygarRoamingState>(m_pFygarObj);
            }
        }
    }

    return nullptr;
}

void digdug::FygarGhostState::StateEnter()
{
    const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("Fygar/Ghost.png") };
    m_pFygarObj->GetComponent<that::SpriteRenderer>()->SetSprite(pTexture, 2, 1, 0.4f);

    m_pGrid = m_pFygarObj->GetParent()->GetComponent<GridComponent>();

    m_pFygar = m_pFygarObj->GetComponent<Fygar>();
}

void digdug::FygarGhostState::StateEnd()
{
}
