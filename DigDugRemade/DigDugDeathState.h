#pragma once

#include "DigDugState.h"

namespace that
{
	class GameObject;
	class SpriteRenderer;
}

namespace digdug
{
	class DigDugDeathState final : public DigDugState
	{
	public:
		DigDugDeathState(that::GameObject* pPlayer);
		virtual ~DigDugDeathState() = default;

		virtual std::unique_ptr<DigDugState> HandleInput(const glm::ivec2& movement, bool pumping, bool pumpHold);
		virtual std::unique_ptr<DigDugState> Update();
		virtual void StateEnter();
		virtual void StateEnd();
	private:
		bool m_WaitingForRespawn{};
		float m_WaitTime{};
		const float m_TimeTillRespawn{ 1.0f };

		that::GameObject* m_pPlayer{};
		that::SpriteRenderer* m_pSprite{};
	};
}
