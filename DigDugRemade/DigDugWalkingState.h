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
		bool m_PrevWalking{};
		bool m_IsWalking{};

		const float m_SpriteSpeed{ 0.2f };

		that::GameObject* m_pPlayer{};
		GridTransform* m_pTransform{};

		std::vector<that::Command*> m_pMoveCommands{};
		std::vector<that::Command*> m_pShootPumpCommands{};
	};
}
