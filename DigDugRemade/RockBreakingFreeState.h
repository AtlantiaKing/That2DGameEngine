#pragma once

#include "State.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class RockBreakingFreeState : public State
	{
	public:
		RockBreakingFreeState(that::GameObject* pRock);
		virtual ~RockBreakingFreeState() = default;

		virtual std::unique_ptr<State> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;
	private:
		that::GameObject* m_pRock{};

		float m_CurTime{};
		const float m_TimeUntilFall{ 0.5f };
	};
}


