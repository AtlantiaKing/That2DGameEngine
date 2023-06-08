#include "HealthComponent.h"

#include "EventQueue.h"
#include "Events.h"

#include "GameObject.h"

void digdug::HealthComponent::Hit()
{
	if (m_Health <= 0) return;
	
	--m_Health;

	OnHealthUpdate.Notify(*this);

	if (m_Health <= 0) 
		Die();
}

void digdug::HealthComponent::Heal()
{
	++m_Health;

	OnHealthUpdate.Notify(*this);
}

void digdug::HealthComponent::Kill()
{
	m_Health = 0;

	OnHealthUpdate.Notify(*this);

	Die();
}

void digdug::HealthComponent::SetMaxHealth(int health)
{
	m_MaxHealth = health;
	m_Health = health;

	OnHealthUpdate.Notify(*this);

	if (m_Health <= 0) Die();
}

void digdug::HealthComponent::Die()
{
	// Notify direct listeners (observers)
	OnDeath.Notify(*GetOwner());
	// Notify non-direct listeners (eventlisteners)
	that::EventQueue::GetInstance().SendEvent(that::EntityDeathEvent{ GetOwner() });

	if (m_ShouldDestroyOnDeath)
	{
		// Destroy the gameobject
		GetOwner()->Destroy();
	}
}
