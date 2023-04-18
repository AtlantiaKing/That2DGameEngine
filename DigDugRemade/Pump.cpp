#include "Pump.h"

#include "GameObject.h"
#include "EnemyMovement.h"
#include "TextureRenderer.h"

#include "Timer.h"
#include "GridTransform.h"
#include "Player.h"
#include "ScoreComponent.h"

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

	// If the pump time is over, disable the pump and enable the player
	if (m_AccuAliveTime <= 0)
	{
		m_IsActive = false;
		GetOwner()->GetComponent<that::TextureRenderer>()->SetEnabled(false);

		GetOwner()->GetParent()->GetComponent<GridTransform>()->SetEnabled(true);
	}
}

void digdug::Pump::Notify(const CollisionData& data)
{
	if (!m_IsActive) return;

	// If the pump hits an enemy, kill it
	HealthComponent* pOtherHealth{ data.other->GetComponent<HealthComponent>() };
	if (pOtherHealth)
	{
		pOtherHealth->Hit();

		if (pOtherHealth->GetHealth() <= 0)
		{
			constexpr int pointPerKill{ 200 };
			GetOwner()->GetParent()->GetComponent<ScoreComponent>()->AddScore(pointPerKill);
		}

		// TODO: A pump will not instantly kill an enemy but slowly inflate it until the enemy bursts.
		//			Score and enemy destroyal should be called when the health of the enemy reaches 0 (the enemy bursts)
		//			An event is called when health reaches zero
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

	// Reset the alive time of the pump
	m_AccuAliveTime = m_AliveTime;
}
