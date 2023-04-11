#pragma once

#include "Component.h"

#include "Event.h"

namespace that
{
	class GameObject;
}

namespace digdug
{

	class EnemyDeathEvent : public that::Event
	{
	public:
		EnemyDeathEvent(that::GameObject* pEnemy = nullptr) : Event{ "EnemyDeath" }, m_pEnemy{ pEnemy } {}
		that::GameObject* GetEnemy() { return m_pEnemy; }
	private:
		that::GameObject* m_pEnemy;
	};

	class Enemy final : public that::Component
	{
	public:
		Enemy() = default;
		virtual ~Enemy() = default;

		Enemy(const Enemy& other) = delete;
		Enemy(Enemy&& other) = delete;
		Enemy& operator=(const Enemy& other) = delete;
		Enemy& operator=(Enemy&& other) = delete;

		void Kill();
	};
}