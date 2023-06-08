#include "PookaRockDeathState.h"

#include "GameObject.h"

#include "HealthComponent.h"
#include "SpriteRenderer.h"

#include "Timer.h"
#include "TextureManager.h"

digdug::PookaRockDeathState::PookaRockDeathState(that::GameObject* pPooka, that::GameObject* pPlayer)
	: m_pPookaObj{ pPooka }
	, m_pPlayer{ pPlayer }
{
}

std::unique_ptr<digdug::EnemyState> digdug::PookaRockDeathState::Update()
{
	m_WaitTime += that::Timer::GetInstance().GetElapsed();

	if (m_WaitTime > m_TimeTillRespawn)
	{
		m_pPookaObj->GetComponent<HealthComponent>()->Kill();
	}

	return nullptr;
}

void digdug::PookaRockDeathState::StateEnter()
{
	const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("Pooka/DeathByRock.png") };
	m_pPookaObj->GetComponent<that::SpriteRenderer>()->SetSprite(pTexture, 1, 1);
}

void digdug::PookaRockDeathState::StateEnd()
{
}
