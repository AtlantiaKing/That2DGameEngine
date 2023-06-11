#pragma once

#include "DigDugState.h"

#include <vector>

namespace that
{
	class GameObject;
	class Command;
}

namespace digdug
{
	class GridTransform;
	class GameState;

	class DigDugWalkingState final : public DigDugState
	{
	public:
		DigDugWalkingState(that::GameObject* pPlayer);
		virtual ~DigDugWalkingState() = default;

		virtual std::unique_ptr<DigDugState> HandleInput(const glm::ivec2& movement, bool pumping, bool pumpHold);
		virtual std::unique_ptr<DigDugState> Update();
		virtual void StateEnter();
		virtual void StateEnd();
	private:
		const float m_SpriteTime{ 0.1f };

		glm::vec2 m_PrevPosition{};
		bool m_IsWalking{};

		that::GameObject* m_pPlayer{};
		GridTransform* m_pTransform{};

		std::vector<that::Command*> m_pMoveCommands{};
		std::vector<that::Command*> m_pShootPumpCommands{};

		GameState* m_pGame{};
	};
}
