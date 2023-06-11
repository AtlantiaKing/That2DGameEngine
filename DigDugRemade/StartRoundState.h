#pragma once

#include "State.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class GameState;

	class StartRoundState final : public State
	{
	public:
		StartRoundState(GameState* pState);
		virtual ~StartRoundState() = default;

		virtual std::unique_ptr<State> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

	private:
		GameState* m_pState{};

		that::GameObject* m_pReadyObject{};

		float m_WaitTimer{};
		const float m_TimeTillStartRound{ 2.5f };
	};
}
