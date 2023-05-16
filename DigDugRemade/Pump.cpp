#include "Pump.h"

#include "GameObject.h"
#include "EnemyMovement.h"
#include "TextureRenderer.h"
#include "Timer.h"

#include "TextureMask.h"
#include "GridTransform.h"
#include "Player.h"
#include "ScoreComponent.h"
#include "BoxCollider.h"

#include <iostream>

void digdug::Pump::Init()
{
	m_pCollider = GetOwner()->GetComponent<that::BoxCollider>();
	m_pCollider->OnHitEvent().AddListener(this);

	// Disable the renderer
	m_pTexture = GetOwner()->GetComponent<that::TextureRenderer>();
	m_pTexture->SetEnabled(false);

	m_pMask = GetOwner()->GetComponent<that::TextureMask>();
}

void digdug::Pump::Update()
{
	// Do nothing if the pump is not active
	if (!m_IsActive) return;

	// Decrease the alive timer only when there is nothing attached to the pump
	if (!m_pPumpTo)
	{
		// Decrease the time that the pump has been enabled
		m_AccuAliveTime -= that::Timer::GetInstance().GetElapsed();
	}

	// Apply the alive timer to the texture mask
	m_pMask->SetPercentage(true, 1.0f - m_AccuAliveTime / m_AliveTime);

	// Scale the collider according to the mask
	const float textureSize{ m_pTexture->GetScaledTextureSize().x };
	const float colliderSize{ m_pMask->GetMask().x * textureSize };
	m_pCollider->SetSize(colliderSize, m_pCollider->GetSize().y);
	m_pCollider->SetCenter(-textureSize / 2.0f + colliderSize / 2.0f, 0.0f);

	// Disable the pump if the timer is less then 0
	if (m_AccuAliveTime < 0.0f)
	{
		DisablePump();
	}

	// If nothing is attached to the pump, stop here
	if (!m_pPumpTo) return;

	// Disable the pump if the enemy has regenerated all its health
	if (m_pPumpTo->GetHealth() == m_pPumpTo->GetMaxHealth())
	{
		DisablePump();
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
	GetOwner()->GetComponent<that::BoxCollider>()->OnHitEvent().RemoveListener(this);
}

void digdug::Pump::Notify(const that::CollisionData& data)
{
	if (!m_IsActive) return;
	if (m_pPumpTo) return;
	if (data.pOther->GetOwner()->HasComponent<Player>()) return;

	// If the pump collider hits an enemy, decrease the health of the enemy and stop the pump animation
	HealthComponent* pOtherHealth{ data.pOther->GetOwner()->GetComponent<HealthComponent>()};
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
	if (!m_CanPump || !m_pPumpTo) return;

	// Hit the enemy
	m_pPumpTo->Hit();

	// Reset the pump time of the pump
	m_AccuPumpTime = m_TimeBetweenPumps;
	m_CanPump = false;

	// Disable the pump if the enemy is dead
	if (m_pPumpTo->GetHealth() <= 0)
	{
		DisablePump();
	}
}

void digdug::Pump::DisablePump()
{
	// Disable the pump logic
	m_IsActive = false;
	// Disable the renderer
	GetOwner()->GetComponent<that::TextureRenderer>()->SetEnabled(false);
	// Enable the player transform
	GetOwner()->GetParent()->GetComponent<GridTransform>()->SetEnabled(true);
	// Remove any attached enemy
	m_pPumpTo = nullptr;
}
