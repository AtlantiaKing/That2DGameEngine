#pragma once

#include "State.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class GameState;

	class EndRoundState final : public State
	{
	public:
		EndRoundState(GameState* pGame, bool victory);
		virtual ~EndRoundState() = default;

		virtual std::unique_ptr<State> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

	private:
		GameState* m_pGame{};

		that::GameObject* m_pGameOver{};

		bool m_Victory{};

		float m_WaitTimer{};
		const float m_TimeTillNextLevel{ 2.5f };
	};
}
