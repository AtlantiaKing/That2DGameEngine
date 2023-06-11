#pragma once

#include "State.h"

#include "glm/vec2.hpp"

#include <memory>

namespace that
{
	class GameObject;
}

namespace digdug
{
	class Fygar;
	class GridComponent;
	class FygarLogic;

	class FygarGhostState : public State
	{
	public:
		FygarGhostState(that::GameObject* pFygar);
		virtual ~FygarGhostState() = default;

		virtual std::unique_ptr<State> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

	private:
		std::unique_ptr<FygarLogic> m_pLogic{};

		that::GameObject* m_pFygarObj{};
		Fygar* m_pFygar{};

		GridComponent* m_pGrid{};

		float m_WaitForCheck{};
		const float m_WaitTime{ 3.0f };

		const float m_GridEpsilon{ 4.0f };
	};
}
