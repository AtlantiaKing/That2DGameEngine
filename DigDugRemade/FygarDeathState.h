#pragma once

#include "State.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class FygarDeathState final : public State
	{
	public:
		FygarDeathState(that::GameObject* pFygar);
		virtual ~FygarDeathState() = default;

		virtual std::unique_ptr<digdug::State> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

	private:
		float m_WaitTime{};
		const float m_TimeTillDespawn{ 0.5f };

		that::GameObject* m_pFygarObj{};
	};
}