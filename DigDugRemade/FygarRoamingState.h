#pragma once

#include "State.h"

#include "glm/vec2.hpp"
#include "FygarLogic.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class GameState;

	class FygarRoamingState final : public State
	{
	public:
		FygarRoamingState(that::GameObject* pFygar);
		virtual ~FygarRoamingState() = default;

		virtual std::unique_ptr<digdug::State> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

	private:
		std::unique_ptr<FygarLogic> m_pLogic{};
		that::GameObject* m_pFygarObj{};

		GameState* m_pGame{};
	};
}