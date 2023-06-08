#include "FygarRockDeathState.h"

#include "GameObject.h"

#include "SpriteRenderer.h"
#include "HealthComponent.h"

#include "TextureManager.h"
#include "Timer.h"

digdug::FygarRockDeathState::FygarRockDeathState(that::GameObject* pFygar, that::GameObject* pPlayer)
	: m_pFygarObj{ pFygar }
	, m_pPlayer{ pPlayer }
{
}

std::unique_ptr<digdug::EnemyState> digdug::FygarRockDeathState::Update()
{
	m_WaitTime += that::Timer::GetInstance().GetElapsed();

	if (m_WaitTime > m_TimeTillRespawn)
	{
		m_pFygarObj->GetComponent<HealthComponent>()->Kill();
	}

	return nullptr;
}

void digdug::FygarRockDeathState::StateEnter()
{
	const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("Fygar/DeathByRock.png") };
	m_pFygarObj->GetComponent<that::SpriteRenderer>()->SetSprite(pTexture, 1, 1);
}

void digdug::FygarRockDeathState::StateEnd()
{
}
