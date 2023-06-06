#pragma once

#include "Component.h"
#include "Observer.h"

#include "PookaState.h"

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
	private:
		void ChangeState(std::unique_ptr<digdug::PookaState> pState);

		that::GameObject* m_pPlayer{};

		std::unique_ptr<PookaState> m_pState{};
	};
}
