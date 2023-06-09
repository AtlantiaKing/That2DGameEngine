#include "PookaRockDeathState.h"

#include "GameObject.h"

#include "HealthComponent.h"
#include "SpriteRenderer.h"
#include "BoxCollider.h"

#include "Timer.h"
#include "TextureManager.h"

digdug::PookaRockDeathState::PookaRockDeathState(that::GameObject* pPooka)
	: m_pPookaObj{ pPooka }
{
}

std::unique_ptr<digdug::EnemyState> digdug::PookaRockDeathState::Update()
{
	m_WaitTime += that::Timer::GetInstance().GetElapsed();

	if (m_WaitTime > m_TimeTillDespawn)
	{
		m_pPookaObj->GetComponent<HealthComponent>()->Kill();
		m_pPookaObj->Destroy();
	}

	return nullptr;
}

void digdug::PookaRockDeathState::StateEnter()
{
	const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("Pooka/DeathByRock.png") };
	m_pPookaObj->GetComponent<that::SpriteRenderer>()->SetSprite(pTexture, 1, 1);

	m_pPookaObj->GetComponent<that::BoxCollider>()->SetEnabled(false);
}

void digdug::PookaRockDeathState::StateEnd()
{
}
