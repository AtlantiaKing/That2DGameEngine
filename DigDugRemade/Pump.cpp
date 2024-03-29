#include "Pump.h"

#include "EnemyMovement.h"
#include "TextureComponent.h"
#include "TextureMask.h"
#include "GridTransform.h"
#include "ScoreComponent.h"
#include "BoxCollider.h"
#include "HealthComponent.h"

#include "GameObject.h"

#include "Timer.h"
#include "ServiceLocator.h"
#include "AudioSystem.h"

#include "ColliderLayers.h"

void digdug::Pump::Init()
{
	m_pCollider = GetOwner()->GetComponent<that::BoxCollider>();
	m_pCollider->OnCollision.AddListener(this);

	// Disable the renderer
	m_pTexture = GetOwner()->GetComponent<that::TextureComponent>();
	m_pTexture->SetEnabled(false);

	m_pMask = GetOwner()->GetComponent<that::TextureMask>();
}

void digdug::Pump::Update()
{
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
		GetOwner()->SetActive(false);
	}

	// If nothing is attached to the pump, stop here
	if (!m_pPumpTo) return;

	// Disable the pump if the enemy has regenerated all its health
	if (m_pPumpTo->GetHealth() == m_pPumpTo->GetMaxHealth())
	{
		GetOwner()->SetActive(false);
	}
}

void digdug::Pump::OnDestroy()
{
	GetOwner()->GetComponent<that::BoxCollider>()->OnCollision.RemoveListener(this);
}

that::GameObject* digdug::Pump::GetTarget() const
{
	if (!m_pPumpTo) return nullptr;

	return m_pPumpTo->GetOwner();
}

void digdug::Pump::Notify(const that::CollisionData& data)
{
	if (!GetOwner()->IsActive()) return;
	if (m_pPumpTo) return;
	if (data.pOther->GetLayer() != ENEMY_LAYER) return;

	// If the pump collider hits an enemy, decrease the health of the enemy and stop the pump animation
	HealthComponent* pOtherHealth{ data.pOther->GetOwner()->GetComponent<HealthComponent>()};
	if (pOtherHealth)
	{
		m_pPumpTo = pOtherHealth;

		PumpToEnemy();
	}
}

void digdug::Pump::OnEnable()
{
	GetOwner()->GetComponent<that::TextureComponent>()->SetEnabled(true);

	m_AccuAliveTime = m_AliveTime;

	// Remove any attached enemy
	m_pPumpTo = nullptr;
}

void digdug::Pump::OnDisable()
{
	// Disable the renderer
	GetOwner()->GetComponent<that::TextureComponent>()->SetEnabled(false);
}

void digdug::Pump::PumpToEnemy()
{
	if (!m_pPumpTo) return;

	// Hit the enemy
	m_pPumpTo->Hit();

	// Disable the pump if the enemy is dead
	if (m_pPumpTo->GetHealth() <= 0)
	{
		GetOwner()->SetActive(false);
		that::ServiceLocator::GetAudio().Play("Sounds/EnemyDeath.wav", 1.0f);
	}
	else
	{
		that::ServiceLocator::GetAudio().Play("Sounds/PumpToEnemy.wav", 1.0f);
	}
}
