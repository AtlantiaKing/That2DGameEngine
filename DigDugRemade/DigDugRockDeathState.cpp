#include "DigDugRockDeathState.h"

#include "GameObject.h"

#include "SpriteRenderer.h"
#include "GridTransform.h"
#include "Transform.h"

#include "TextureManager.h"
#include "Timer.h"

#include "DigDugWalkingState.h"

digdug::DigDugRockDeathState::DigDugRockDeathState(that::GameObject* pPlayer)
	: m_pPlayer{ pPlayer }
{
}

std::unique_ptr<digdug::DigDugState> digdug::DigDugRockDeathState::HandleInput(const glm::ivec2&, bool, bool)
{
	return nullptr;
}

std::unique_ptr<digdug::DigDugState> digdug::DigDugRockDeathState::Update()
{
	m_WaitTime += that::Timer::GetInstance().GetElapsed();

	if (m_WaitTime > m_TimeTillRespawn)
	{
		m_pPlayer->GetComponent<GridTransform>()->SetPosition(0, 0);
		m_pPlayer->GetTransform()->SetLocalPosition(0.0f, 0.0f);
		return std::make_unique<DigDugWalkingState>(m_pPlayer);
	}

	return nullptr;
}

void digdug::DigDugRockDeathState::StateEnter()
{
	const auto& pPlayerTexture{ that::TextureManager::GetInstance().LoadTexture("DigDug/DeathByRock.png") };
	m_pPlayer->GetComponent<that::SpriteRenderer>()->SetSprite(pPlayerTexture, 1, 1);
}

void digdug::DigDugRockDeathState::StateEnd()
{
}
