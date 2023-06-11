#include "FygarRockDeathState.h"

#include "GameObject.h"

#include "SpriteRenderer.h"
#include "HealthComponent.h"
#include "BoxCollider.h"

#include "TextureManager.h"
#include "Timer.h"

digdug::FygarRockDeathState::FygarRockDeathState(that::GameObject* pFygar)
	: m_pFygarObj{ pFygar }
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

	m_pFygarObj->GetComponent<that::BoxCollider>()->SetEnabled(false);
}

void digdug::FygarRockDeathState::StateEnd()
{
}
