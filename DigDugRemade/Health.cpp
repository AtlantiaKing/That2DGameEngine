#include "Health.h"

#include "EventQueue.h"
#include "Events.h"

void digdug::Health::Hit()
{
	if (m_Health <= 0) return;
	
	--m_Health;

	if (m_Health <= 0) that::EventQueue::GetInstance().SendEvent(that::EntityDeathEvent{GetOwner()});
}

void digdug::Health::Heal()
{
	++m_Health;
}

void digdug::Health::SetHealth(int health)
{
	m_Health = health;

	if (m_Health <= 0) that::EventQueue::GetInstance().SendEvent(that::EntityDeathEvent{ GetOwner() });
}
