#pragma once

#include "Component.h"
#include "Subject.h"

namespace digdug
{
	class HealthComponent final : public that::Component
	{
	public:
		HealthComponent() = default;
		virtual ~HealthComponent() = default;

		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		void Hit();
		void Heal();
		void Kill();

		void SetDestroyOnDeath(bool shouldDestroyOnDeath) { m_ShouldDestroyOnDeath = shouldDestroyOnDeath; }
		void SetMaxHealth(int health);

		int GetHealth() const { return m_Health; }
		int GetMaxHealth() const { return m_MaxHealth; }

		that::Subject<HealthComponent> OnHealthUpdate{};
		that::Subject<that::GameObject> OnDeath{};
	private:
		void Die();

		int m_Health{};
		int m_MaxHealth{};

		bool m_ShouldDestroyOnDeath{ true };
	};
}