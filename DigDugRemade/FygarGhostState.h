#pragma once

#include "FygarState.h"

#include "glm/vec2.hpp"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class GridComponent;

	class FygarGhostState : public FygarState
	{
	public:
		FygarGhostState(that::GameObject* pFygar, that::GameObject* pPlayer);
		virtual ~FygarGhostState() = default;

		virtual std::unique_ptr<FygarState> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

	private:
		that::GameObject* m_pFygarObj{};
		that::GameObject* m_pPlayer{};

		GridComponent* m_pGrid{};
		glm::ivec2 m_GoToTile{};

		float m_WaitForCheck{};
		const float m_WaitTime{ 3.0f };

		const float m_MoveSpeed{ 25.0f };

		const float m_GridEpsilon{ 4.0f };
	};
}
