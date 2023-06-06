#include "FygarPumpState.h"

#include "GameObject.h"

#include "HealthComponent.h"
#include "TextureRenderer.h"
#include "BoxCollider.h"
#include "Transform.h"

#include "FygarRoamingState.h"

#include "Timer.h"
#include "ResourceManager.h"

digdug::FygarPumpState::FygarPumpState(that::GameObject* pFygar, that::GameObject* pPlayer)
	: m_pFygarObj{ pFygar }
	, m_pPlayer{ pPlayer }
{
}

std::unique_ptr<digdug::FygarState> digdug::FygarPumpState::Update()
{
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
	m_pFygarObj->GetComponent<HealthComponent>()->OnHealthUpdate().AddListener(this);

	const auto& pTexture{ that::ResourceManager::GetInstance().LoadTexture("Fygar/Pumped.png") };
	m_pFygarObj->GetComponent<that::TextureRenderer>()->SetTexture(pTexture);

	m_pFygarObj->GetComponent<that::BoxCollider>()->SetEnabled(false);
}

void digdug::FygarPumpState::StateEnd()
{
	m_pFygarObj->GetComponent<HealthComponent>()->OnHealthUpdate().RemoveListener(this);

	m_pFygarObj->GetComponent<that::BoxCollider>()->SetEnabled(true);
}

void digdug::FygarPumpState::Notify(const HealthComponent& health)
{
	m_DeflateTime = 0.0f;

	if (health.GetMaxHealth() != health.GetHealth())
		m_pFygarObj->GetTransform()->SetLocalScale(1.0f + (health.GetMaxHealth() - health.GetHealth() - 1) / 2.0f);
}
