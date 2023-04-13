#pragma once

#include "Event.h"

namespace that
{
	class GameObject;

	struct EntityDeathEvent final : public Event
	{
		EntityDeathEvent(GameObject* _pEntity = nullptr) : Event{ "EntityDeath" }, pEntity{ _pEntity } {}
		virtual ~EntityDeathEvent() = default;

		GameObject* pEntity{};
	};
}