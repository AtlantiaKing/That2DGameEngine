#pragma once

#include "EnemyState.h"
#include "Observer.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class HealthComponent;

	class PookaPumpState : public EnemyState, public that::Observer<HealthComponent>
	{
	public:
		PookaPumpState(that::GameObject* pPooka, that::GameObject* pPlayer);
		virtual ~PookaPumpState() = default;

		virtual std::unique_ptr<EnemyState> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

		virtual void Notify(const HealthComponent& health) override;

	private:
		float m_DeflateTime{};
		const float m_TimeUntilDeflate{ 1.0f };
		bool m_Dead{};

		that::GameObject* m_pPookaObj{};
		that::GameObject* m_pPlayer{};
	};
}