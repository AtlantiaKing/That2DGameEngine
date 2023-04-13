#pragma once

#include "Event.h"

namespace that
{
	class GameObject;

	class EntityDeathEvent final : public Event
	{
	public:
		EntityDeathEvent() = default;
		EntityDeathEvent(GameObject* pEntity) : Event{ "EntityDeath" }, m_pEntity{ pEntity } {}
		virtual ~EntityDeathEvent() = default;;
		
		GameObject* GetEntity() { return m_pEntity; }
	private:
		GameObject* m_pEntity{};
	};
}