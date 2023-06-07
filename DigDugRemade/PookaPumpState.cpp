#include "PookaPumpState.h"

#include "GameObject.h"

#include "HealthComponent.h"
#include "SpriteRenderer.h"
#include "BoxCollider.h"
#include "Transform.h"

#include "PookaRoamingState.h"

#include "Timer.h"
#include "TextureManager.h"

digdug::PookaPumpState::PookaPumpState(that::GameObject* pPooka, that::GameObject* pPlayer)
	: m_pPookaObj{ pPooka }
	, m_pPlayer{ pPlayer }
{
}

std::unique_ptr<digdug::EnemyState> digdug::PookaPumpState::Update()
{
	m_DeflateTime += that::Timer::GetInstance().GetElapsed();
	if (m_DeflateTime > m_TimeUntilDeflate)
	{
		HealthComponent* pHealth{ m_pPookaObj->GetComponent<HealthComponent>() };

		pHealth->Heal();

		if (pHealth->GetHealth() == pHealth->GetMaxHealth())
		{
			return std::make_unique<PookaRoamingState>(m_pPookaObj, m_pPlayer);
		}
	}

	return nullptr;
}

void digdug::PookaPumpState::StateEnter()
{
	m_pPookaObj->GetComponent<HealthComponent>()->OnHealthUpdate().AddListener(this);

	const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("Pooka/Pumped.png") };
	m_pPookaObj->GetComponent<that::SpriteRenderer>()->SetSprite(pTexture, 4, 1);

	m_pPookaObj->GetComponent<that::BoxCollider>()->SetEnabled(false);
}

void digdug::PookaPumpState::StateEnd()
{
	m_pPookaObj->GetComponent<HealthComponent>()->OnHealthUpdate().RemoveListener(this);

	m_pPookaObj->GetComponent<that::BoxCollider>()->SetEnabled(true);
}

void digdug::PookaPumpState::Notify(const HealthComponent& health)
{
	m_DeflateTime = 0.0f;

	if (health.GetMaxHealth() != health.GetHealth())
	{
		const int tileIdx{ health.GetMaxHealth() - health.GetHealth() - 1 };
		m_pPookaObj->GetComponent<that::SpriteRenderer>()->SetTile(tileIdx);
	}
}
