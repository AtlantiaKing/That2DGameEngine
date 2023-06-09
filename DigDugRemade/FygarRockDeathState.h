#pragma once

#include "EnemyState.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class FygarRockDeathState final : public EnemyState
	{
	public:
		FygarRockDeathState(that::GameObject* pFygar);
		virtual ~FygarRockDeathState() = default;

		virtual std::unique_ptr<digdug::EnemyState> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

	private:
		float m_WaitTime{};
		const float m_TimeTillRespawn{ 1.0f };

		that::GameObject* m_pFygarObj{};
	};
}