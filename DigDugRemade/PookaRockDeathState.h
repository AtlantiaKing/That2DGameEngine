#pragma once

#include "EnemyState.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class PookaRockDeathState final : public EnemyState
	{
	public:
		PookaRockDeathState(that::GameObject* pPooka);
		virtual ~PookaRockDeathState() = default;

		virtual std::unique_ptr<digdug::EnemyState> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

	private:
		float m_WaitTime{};
		const float m_TimeTillDespawn{ 1.0f };

		that::GameObject* m_pPookaObj{};
	};
}