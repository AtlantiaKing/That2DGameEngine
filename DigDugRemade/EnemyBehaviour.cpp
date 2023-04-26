#include "EnemyBehaviour.h"

#include "GameObject.h"
#include "Timer.h"

#include "EnemyMovement.h"
#include "TextureRenderer.h"
#include "GridCollider.h"

void digdug::EnemyBehaviour::Init()
{
	m_pHealth = GetOwner()->GetComponent<HealthComponent>();
	m_pHealth->OnHealthUpdate().AddListener(this);

	m_pMovement = GetOwner()->GetComponent<EnemyMovement>();

	m_pTexture = GetOwner()->GetComponent<that::TextureRenderer>();
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
	m_pHealth->OnHealthUpdate().RemoveListener(this);
}

void digdug::EnemyBehaviour::Notify(const HealthComponent& health)
{
	if (m_pHealth->GetHealth() == m_pHealth->GetMaxHealth())
	{
		m_pMovement->SetEnabled(true);
		m_pHealth->GetOwner()->GetComponent<GridCollider>()->SetEnabled(true);
	}
	else
	{
		m_pMovement->SetEnabled(false);
		m_pHealth->GetOwner()->GetComponent<GridCollider>()->SetEnabled(false);

		m_AccuHealTime = m_TimeBetweenHeals;

		m_pTexture->SetScale(2.0f + std::max(health.GetMaxHealth() - health.GetHealth() - 1, 0));
	}
}