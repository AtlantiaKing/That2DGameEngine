#include "Pump.h"

#include "GameObject.h"
#include "EnemyMovement.h"
#include "TextureRenderer.h"

#include "Timer.h"
#include "GridTransform.h"
#include "Player.h"
#include "ScoreComponent.h"
#include <iostream>

void digdug::Pump::Init()
{
	GetOwner()->GetComponent<GridCollider>()->OnCollision().AddListener(this);

	// Disable the renderer
	GetOwner()->GetComponent<that::TextureRenderer>()->SetEnabled(false);
}

void digdug::Pump::Update()
{
	// Do nothing if the pump is not active
	if (!m_IsActive) return;

	// Decrease the time that the pump has been enabled
	m_AccuAliveTime -= that::Timer::GetInstance().GetElapsed();

	if (m_AccuAliveTime < 0.0f)
	{
		m_IsActive = false;
		GetOwner()->GetComponent<that::TextureRenderer>()->SetEnabled(false);

		GetOwner()->GetParent()->GetComponent<GridTransform>()->SetEnabled(true);

		m_pPumpTo = nullptr;
	}

	if (!m_pPumpTo) return;

	// Disable the pump if the enemy has regenerated all its health
	if (m_pPumpTo->GetHealth() == m_pPumpTo->GetMaxHealth())
	{
		m_IsActive = false;
		GetOwner()->GetComponent<that::TextureRenderer>()->SetEnabled(false);

		GetOwner()->GetParent()->GetComponent<GridTransform>()->SetEnabled(true);

		m_pPumpTo = nullptr;
	}

	// Set CanPump to true if AccuPumpTime is 0
	if (m_AccuPumpTime > 0)
	{
		// Decrease the time that the pump has been enabled
		m_AccuPumpTime -= that::Timer::GetInstance().GetElapsed();

		// If the pump time is over, disable the pump and enable the player
		if (m_AccuPumpTime <= 0)
		{
			m_CanPump = true;
		}
	}
}

void digdug::Pump::OnDestroy()
{
	GetOwner()->GetComponent<GridCollider>()->OnCollision().RemoveListener(this);
}

void digdug::Pump::Notify(const CollisionData& data)
{
	if (!m_IsActive) return;
	if (m_pPumpTo) return;

	// If the pump hits an enemy, kill it
	HealthComponent* pOtherHealth{ data.other->GetComponent<HealthComponent>() };
	if (pOtherHealth)
	{
		m_pPumpTo = pOtherHealth;

		m_CanPump = true;
		PumpToEnemy();
	}
}

void digdug::Pump::Enable()
{
	// If the pump is already active, don't reactivate
	if (m_IsActive) return;

	// Activate the pump
	m_IsActive = true;
	GetOwner()->GetComponent<that::TextureRenderer>()->SetEnabled(true);

	// Disable the movement of the player
	GetOwner()->GetParent()->GetComponent<GridTransform>()->SetEnabled(false);

	m_AccuAliveTime = m_AliveTime;
}

void digdug::Pump::PumpToEnemy()
{
	if (!m_CanPump) return;

	// Hit the enemy
	m_pPumpTo->Hit();

	std::cout << "hit\n";

	// Reset the pump time of the pump
	m_AccuPumpTime = m_TimeBetweenPumps;
	m_AccuAliveTime = m_AliveTime;
	m_CanPump = false;
}
