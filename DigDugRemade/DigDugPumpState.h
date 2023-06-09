#pragma once

#include "DigDugState.h"

namespace that
{
	class GameObject;
	class SpriteRenderer;
	class Command;
}

namespace digdug
{
	class Pump;

	class DigDugPumpState final : public DigDugState
	{
	public:
		DigDugPumpState(that::GameObject* pPlayer);
		virtual ~DigDugPumpState() = default;

		virtual std::unique_ptr<DigDugState> HandleInput(const glm::ivec2& movement, bool pumping, bool pumpHold);
		virtual std::unique_ptr<DigDugState> Update();
		virtual void StateEnter();
		virtual void StateEnd();
	private:
		bool m_PumpedThisFrame{};
		const float m_TimeBetweenPumps{ 0.6f };
		const float m_ResetSpriteTime{ 0.3f };
		float m_PumpTime{};

		glm::ivec2 m_Movement{};

		that::GameObject* m_pPlayer{};
		Pump* m_pPump{};
		that::SpriteRenderer* m_pSprite{};

		that::Command* m_pPumpEnemyCommand{};
	};
}
