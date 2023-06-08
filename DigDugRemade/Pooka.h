#pragma once

#include "Component.h"
#include "Observer.h"

#include "EnemyState.h"

namespace digdug
{
	class HealthComponent;

	class Pooka final : public that::Component, public that::Observer<HealthComponent>
	{
	public:
		Pooka() = default;
		virtual ~Pooka() = default;

		Pooka(const Pooka& other) = delete;
		Pooka(Pooka&& other) = delete;
		Pooka& operator=(const Pooka& other) = delete;
		Pooka& operator=(Pooka&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;
		virtual void Notify(const HealthComponent& health) override;

		void Start(that::GameObject* pPlayer);
		void RockAttack();
	private:
		void ChangeState(std::unique_ptr<digdug::EnemyState> pState);

		that::GameObject* m_pPlayer{};
			
		std::unique_ptr<EnemyState> m_pState{};
	};
}
