#pragma once

#include "State.h"
#include "Observer.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class GameState;

	class InGameState final : public State, public that::Observer<that::GameObject>
	{
	public:
		InGameState(GameState* pGame);
		virtual ~InGameState() = default;

		virtual std::unique_ptr<State> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

		virtual void Notify(const that::GameObject& pEntity);

	private:
		GameState* m_pGame{};

		bool m_Victory{};
		bool m_GameOver{};
	};
}
