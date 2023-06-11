#include "FygarGhostAI.h"

#include "GameObject.h"

#include "Fygar.h"
#include "Transform.h"

#include "Timer.h"

digdug::FygarGhostAI::FygarGhostAI(that::GameObject* pFygar)
    : m_pFygarObj{ pFygar }
    , m_pFygar{ pFygar->GetComponent<Fygar>() }
{
}

std::unique_ptr<digdug::State> digdug::FygarGhostAI::Update()
{
    that::Transform* pTransform{ m_pFygarObj->GetTransform() };
    const auto& pos{ pTransform->GetLocalPosition() };
    const auto& playerPos{ m_pFygar->GetPlayer()->GetTransform()->GetLocalPosition() };

    const glm::vec2 direction{ playerPos - pos };

    const float elapsedSec{ that::Timer::GetInstance().GetElapsed() };

    pTransform->Translate(glm::normalize(direction) * m_MoveSpeed * elapsedSec);

    return nullptr;
}
