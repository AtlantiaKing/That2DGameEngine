#pragma once

#include "EnemyState.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class FygarDeathState final : public EnemyState
	{
	public:
		FygarDeathState(that::GameObject* pFygar, that::GameObject* pPlayer);
		virtual ~FygarDeathState() = default;

		virtual std::unique_ptr<digdug::EnemyState> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

	private:
		float m_WaitTime{};
		const float m_TimeTillDespawn{ 0.5f };

		that::GameObject* m_pFygarObj{};
		that::GameObject* m_pPlayer{};
	};
}