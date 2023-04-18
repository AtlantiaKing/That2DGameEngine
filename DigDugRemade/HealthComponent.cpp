#include "HealthComponent.h"

#include "EventQueue.h"
#include "Events.h"

#include "GameObject.h"

void digdug::HealthComponent::Hit()
{
	if (m_Health <= 0) return;
	
	--m_Health;

	m_HealthUpdateListeners.Notify(*this);

	if (m_Health <= 0) Die();
}

void digdug::HealthComponent::Heal()
{
	++m_Health;

	m_HealthUpdateListeners.Notify(*this);
}

void digdug::HealthComponent::SetHealth(int health)
{
	m_Health = health;

	m_HealthUpdateListeners.Notify(*this);

	if (m_Health <= 0) m_DeathListeners.Notify(*GetOwner());
}

void digdug::HealthComponent::Die()
{
	// Notify direct listeners (observers)
	m_DeathListeners.Notify(*GetOwner());
	// Notify non-direct listeners (eventlisteners)
	that::EventQueue::GetInstance().SendEvent(that::EntityDeathEvent{ GetOwner() });

	if (m_ShouldDestroyOnDeath)
	{
		// Destroy the gameobject
		GetOwner()->Destroy();
	}
}
