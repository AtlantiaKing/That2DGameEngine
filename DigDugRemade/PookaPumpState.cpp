#include "PookaPumpState.h"

#include "GameObject.h"

#include "HealthComponent.h"
#include "SpriteRenderer.h"
#include "BoxCollider.h"

#include "PookaRoamingState.h"
#include "PookaDeathState.h"

#include "Timer.h"
#include "TextureManager.h"

#include "ColliderLayers.h"

digdug::PookaPumpState::PookaPumpState(that::GameObject* pPooka)
	: m_pPookaObj{ pPooka }
{
}

std::unique_ptr<digdug::EnemyState> digdug::PookaPumpState::Update()
{
	if (m_Dead) return std::make_unique<PookaDeathState>(m_pPookaObj);

	m_DeflateTime += that::Timer::GetInstance().GetElapsed();
	if (m_DeflateTime > m_TimeUntilDeflate)
	{
		HealthComponent* pHealth{ m_pPookaObj->GetComponent<HealthComponent>() };

		pHealth->Heal();

		if (pHealth->GetHealth() == pHealth->GetMaxHealth())
		{
			return std::make_unique<PookaRoamingState>(m_pPookaObj);
		}
	}

	return nullptr;
}

void digdug::PookaPumpState::StateEnter()
{
	m_pPookaObj->GetComponent<HealthComponent>()->OnHealthUpdate.AddListener(this);

	const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("Pooka/Pumped.png") };
	m_pPookaObj->GetComponent<that::SpriteRenderer>()->SetSprite(pTexture, 4, 1);

	m_pPookaObj->GetComponent<that::BoxCollider>()->SetIgnoreGroup(DIGDUG_LAYER);
}

void digdug::PookaPumpState::StateEnd()
{
	m_pPookaObj->GetComponent<HealthComponent>()->OnHealthUpdate.RemoveListener(this);

	m_pPookaObj->GetComponent<that::BoxCollider>()->SetIgnoreGroup(0);
}

void digdug::PookaPumpState::Notify(const HealthComponent& health)
{
	m_DeflateTime = 0.0f;

	if (health.GetMaxHealth() != health.GetHealth())
	{
		const int tileIdx{ health.GetMaxHealth() - health.GetHealth() - 1 };
		m_pPookaObj->GetComponent<that::SpriteRenderer>()->SetTile(tileIdx);
	}

	if (health.GetHealth() == 0) m_Dead = true;
}
