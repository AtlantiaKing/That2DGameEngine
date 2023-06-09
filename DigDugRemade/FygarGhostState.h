#pragma once

#include "EnemyState.h"

#include "glm/vec2.hpp"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class Fygar;
	class GridComponent;

	class FygarGhostState : public EnemyState
	{
	public:
		FygarGhostState(that::GameObject* pFygar);
		virtual ~FygarGhostState() = default;

		virtual std::unique_ptr<EnemyState> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

	private:
		that::GameObject* m_pFygarObj{};
		Fygar* m_pFygar{};

		GridComponent* m_pGrid{};

		float m_WaitForCheck{};
		const float m_WaitTime{ 3.0f };

		const float m_MoveSpeed{ 25.0f };

		const float m_GridEpsilon{ 4.0f };
	};
}
