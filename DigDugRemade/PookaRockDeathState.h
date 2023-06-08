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
		PookaRockDeathState(that::GameObject* pPooka, that::GameObject* pPlayer);
		virtual ~PookaRockDeathState() = default;

		virtual std::unique_ptr<digdug::EnemyState> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

	private:
		float m_WaitTime{};
		const float m_TimeTillRespawn{ 1.0f };

		that::GameObject* m_pPookaObj{};
		that::GameObject* m_pPlayer{};
	};
}