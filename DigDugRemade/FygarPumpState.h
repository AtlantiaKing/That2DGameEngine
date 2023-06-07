#pragma once

#include "State.h"
#include "Observer.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class HealthComponent;

	class FygarPumpState : public State, public that::Observer<HealthComponent>
	{
	public:
		FygarPumpState(that::GameObject* pFygar, that::GameObject* pPlayer);
		virtual ~FygarPumpState() = default;

		virtual std::unique_ptr<State> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

		virtual void Notify(const HealthComponent& health) override;

	private:
		float m_DeflateTime{};
		const float m_TimeUntilDeflate{ 1.0f };

		that::GameObject* m_pFygarObj{};
		that::GameObject* m_pPlayer{};
	};
}