#include "RockBreakingFreeState.h"

#include "GameObject.h"

#include "SpriteRenderer.h"

#include "TextureManager.h"
#include "Timer.h"

#include "RockFallingState.h"

digdug::RockBreakingFreeState::RockBreakingFreeState(that::GameObject* pRock, that::GameObject* pPlayer)
	: m_pRock{ pRock }
	, m_pPlayer{ pPlayer }
{
}

std::unique_ptr<digdug::EnemyState> digdug::RockBreakingFreeState::Update()
{
	m_CurTime += that::Timer::GetInstance().GetElapsed();
	if (m_CurTime > m_TimeUntilFall) return std::make_unique<RockFallingState>(m_pRock, m_pPlayer);

	return nullptr;
}

void digdug::RockBreakingFreeState::StateEnter()
{
	const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("Rock/AlmostFalling.png") };
	m_pRock->GetComponent<that::SpriteRenderer>()->SetSprite(pTexture, 2, 1, 0.2f);
}

void digdug::RockBreakingFreeState::StateEnd()
{
}
