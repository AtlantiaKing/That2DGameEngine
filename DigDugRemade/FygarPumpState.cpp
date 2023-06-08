#include "FygarPumpState.h"

#include "GameObject.h"

#include "HealthComponent.h"
#include "SpriteRenderer.h"
#include "BoxCollider.h"
#include "Transform.h"
#include "FygarDeathState.h"

#include "FygarRoamingState.h"

#include "Timer.h"
#include "TextureManager.h"

digdug::FygarPumpState::FygarPumpState(that::GameObject* pFygar, that::GameObject* pPlayer)
	: m_pFygarObj{ pFygar }
	, m_pPlayer{ pPlayer }
{
}

std::unique_ptr<digdug::EnemyState> digdug::FygarPumpState::Update()
{
	if (m_Dead) return std::make_unique<FygarDeathState>(m_pFygarObj, m_pPlayer);

	m_DeflateTime += that::Timer::GetInstance().GetElapsed();
	if (m_DeflateTime > m_TimeUntilDeflate)
	{
		HealthComponent* pHealth{ m_pFygarObj->GetComponent<HealthComponent>() };

		pHealth->Heal();

		if (pHealth->GetHealth() == pHealth->GetMaxHealth())
		{
			return std::make_unique<FygarRoamingState>(m_pFygarObj, m_pPlayer);
		}
	}

	return nullptr;
}

void digdug::FygarPumpState::StateEnter()
{
	m_pFygarObj->GetComponent<HealthComponent>()->OnHealthUpdate.AddListener(this);

	const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("Fygar/Pumped.png") };
	m_pFygarObj->GetComponent<that::SpriteRenderer>()->SetSprite(pTexture, 4, 1);

	m_pFygarObj->GetComponent<that::BoxCollider>()->SetEnabled(false);
}

void digdug::FygarPumpState::StateEnd()
{
	m_pFygarObj->GetComponent<HealthComponent>()->OnHealthUpdate.RemoveListener(this);

	m_pFygarObj->GetComponent<that::BoxCollider>()->SetEnabled(true);
}

void digdug::FygarPumpState::Notify(const HealthComponent& health)
{
	m_DeflateTime = 0.0f;

	if (health.GetMaxHealth() != health.GetHealth())
	{
		const int tileIdx{ health.GetMaxHealth() - health.GetHealth() - 1 };
		m_pFygarObj->GetComponent<that::SpriteRenderer>()->SetTile(tileIdx);
	}

	if (health.GetHealth() == 0) m_Dead = true;
}
