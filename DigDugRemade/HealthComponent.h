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

		void Hit();
		void Heal();
		void Kill();

		void SetDestroyOnDeath(bool shouldDestroyOnDeath) { m_ShouldDestroyOnDeath = shouldDestroyOnDeath; }
		void SetMaxHealth(int health);

		int GetHealth() const { return m_Health; }
		int GetMaxHealth() const { return m_MaxHealth; }

		that::Subject<HealthComponent>& OnHealthUpdate() { return m_HealthUpdateListeners; }
		that::Subject<that::GameObject> OnDeath() { return m_DeathListeners; }
	private:
		void Die();

		that::Subject<HealthComponent> m_HealthUpdateListeners{};
		that::Subject<that::GameObject> m_DeathListeners{};
		int m_Health{};
		int m_MaxHealth{};

		bool m_ShouldDestroyOnDeath{ true };
	};
}