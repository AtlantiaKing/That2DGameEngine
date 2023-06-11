#pragma once

#include "State.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class FygarRockDeathState final : public State
	{
	public:
		FygarRockDeathState(that::GameObject* pFygar);
		virtual ~FygarRockDeathState() = default;

		virtual std::unique_ptr<digdug::State> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

	private:
		float m_WaitTime{};
		const float m_TimeTillRespawn{ 1.0f };

		that::GameObject* m_pFygarObj{};
	};
}