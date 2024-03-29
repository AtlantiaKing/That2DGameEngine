#include "EnemyBehaviour.h"

#include "GameObject.h"
#include "Timer.h"

#include "EnemyMovement.h"
#include "TextureComponent.h"
#include "BoxCollider.h"
#include "Transform.h"

void digdug::EnemyBehaviour::Init()
{
	m_pHealth = GetOwner()->GetComponent<HealthComponent>();
	m_pHealth->OnHealthUpdate.AddListener(this);

	m_pMovement = GetOwner()->GetComponent<EnemyMovement>();

	m_pTexture = GetOwner()->GetComponent<that::TextureComponent>();
}

void digdug::EnemyBehaviour::Update()
{
	if (m_pHealth->GetHealth() == m_pHealth->GetMaxHealth()) return;

	m_AccuHealTime -= that::Timer::GetInstance().GetElapsed();

	if (m_AccuHealTime < 0.0f)
	{
		m_AccuHealTime += m_TimeBetweenHeals;
		m_pHealth->Heal();
	}
}

void digdug::EnemyBehaviour::OnDestroy()
{
	m_pHealth->OnHealthUpdate.RemoveListener(this);
}

void digdug::EnemyBehaviour::Notify(const HealthComponent& health)
{
	const bool hasRegerated{ m_pHealth->GetHealth() == m_pHealth->GetMaxHealth() };

	m_pMovement->SetEnabled(hasRegerated);

	if (!hasRegerated)
	{
		m_AccuHealTime = m_TimeBetweenHeals;

		GetTransform()->SetLocalScale(1.0f + std::max(health.GetMaxHealth() - health.GetHealth() - 1, 0));
	}
}
