#include "PookaDeathState.h"

#include "GameObject.h"

#include "HealthComponent.h"
#include "SpriteRenderer.h"
#include "BoxCollider.h"

#include "Timer.h"
#include "TextureManager.h"

digdug::PookaDeathState::PookaDeathState(that::GameObject* pPooka)
	: m_pPookaObj{ pPooka }
{
}

std::unique_ptr<digdug::EnemyState> digdug::PookaDeathState::Update()
{
	m_WaitTime += that::Timer::GetInstance().GetElapsed();

	if (m_WaitTime > m_TimeTillDespawn)
	{
		m_pPookaObj->Destroy();
	}

	return nullptr;
}

void digdug::PookaDeathState::StateEnter()
{
	const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("Pooka/Pumped.png") };
	that::SpriteRenderer* pSprite{ m_pPookaObj->GetComponent<that::SpriteRenderer>() };
	pSprite->SetSprite(pTexture, 4, 1);
	pSprite->SetTile(3);

	m_pPookaObj->GetComponent<that::BoxCollider>()->SetEnabled(false);
}

void digdug::PookaDeathState::StateEnd()
{
}
