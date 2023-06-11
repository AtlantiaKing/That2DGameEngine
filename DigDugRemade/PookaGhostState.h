#pragma once

#include "State.h"

#include "glm/vec2.hpp"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class Pooka;
	class GridComponent;

	class PookaGhostState : public State
	{
	public:
		PookaGhostState(that::GameObject* pPooka);
		virtual ~PookaGhostState() = default;

		virtual std::unique_ptr<State> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

	private:
		that::GameObject* m_pPookaObj{};
		Pooka* m_pPooka{};

		GridComponent* m_pGrid{};

		float m_WaitForCheck{};
		const float m_WaitTime{ 1.0f };

		const float m_MoveSpeed{ 25.0f };

		const float m_GridEpsilon{ 4.0f };
	};
}
