#pragma once

#include "Event.h"

namespace that
{
	class GameObject;

	class EntityDeathEvent final : public Event
	{
	public:
		EntityDeathEvent(GameObject* pEntity = nullptr) : Event{ "EntityDeath" }, m_pEntity{ pEntity } {}
		virtual ~EntityDeathEvent() = default;
		
		GameObject* GetEntity() { return m_pEntity; }
	private:
		GameObject* m_pEntity{};
	};
}