#pragma once

#include "PookaState.h"
#include "Observer.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class HealthComponent;

	class PookaPumpState : public PookaState, public that::Observer<HealthComponent>
	{
	public:
		PookaPumpState(that::GameObject* pPooka, that::GameObject* pPlayer);
		virtual ~PookaPumpState() = default;

		virtual std::unique_ptr<PookaState> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

		virtual void Notify(const HealthComponent& health) override;

	private:
		float m_DeflateTime{};
		const float m_TimeUntilDeflate{ 1.0f };

		that::GameObject* m_pPookaObj{};
		that::GameObject* m_pPlayer{};
	};
}