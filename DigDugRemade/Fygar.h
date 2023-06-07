#pragma once

#include "Component.h"
#include "Observer.h"

#include "EnemyState.h"

namespace digdug
{
	class HealthComponent;

	class Fygar final : public that::Component, public that::Observer<HealthComponent>
	{
	public:
		Fygar() = default;
		virtual ~Fygar() = default;

		Fygar(const Fygar& other) = delete;
		Fygar(Fygar&& other) = delete;
		Fygar& operator=(const Fygar& other) = delete;
		Fygar& operator=(Fygar&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;
		virtual void Notify(const HealthComponent& health) override;

		void Start(that::GameObject* pPlayer);
	private:
		void ChangeState(std::unique_ptr<digdug::EnemyState> pState);

		that::GameObject* m_pPlayer{};

		std::unique_ptr<EnemyState> m_pState{};
	};
}
