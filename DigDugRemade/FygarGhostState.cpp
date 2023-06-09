#include "FygarGhostState.h"

#include "GameObject.h"

#include "SpriteRenderer.h"
#include "GridComponent.h"
#include "WorldTile.h"
#include "Transform.h"
#include "Fygar.h"

#include "FygarLogic.h"
#include "FygarGhostAI.h"
#include "FygarGhostPlayer.h"

#include "FygarRoamingState.h"

#include "TextureManager.h"
#include "Timer.h"

digdug::FygarGhostState::FygarGhostState(that::GameObject* pFygar)
    : m_pFygarObj{ pFygar }
{
}

std::unique_ptr<digdug::EnemyState> digdug::FygarGhostState::Update()
{
    m_pLogic->Update();

    const float elapsedSec{ that::Timer::GetInstance().GetElapsed() };
    const auto& pos{ m_pFygarObj->GetTransform()->GetLocalPosition()};
    
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

    //m_pLogic = std::make_unique<FygarGhostAI>(m_pFygarObj);
    m_pLogic = std::make_unique<FygarGhostPlayer>(m_pFygarObj);
}

void digdug::FygarGhostState::StateEnd()
{
}
