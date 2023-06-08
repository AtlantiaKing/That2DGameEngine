#pragma once

#include "DigDugState.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class DigDugRockDeathState final : public DigDugState
	{
	public:
		DigDugRockDeathState(that::GameObject* pPlayer);
		virtual ~DigDugRockDeathState() = default;

		virtual std::unique_ptr<DigDugState> HandleInput(const glm::ivec2& movement, bool pumping, bool pumpHold);
		virtual std::unique_ptr<DigDugState> Update();
		virtual void StateEnter();
		virtual void StateEnd();
	private:
		float m_WaitTime{};
		const float m_TimeTillRespawn{ 1.0f };

		that::GameObject* m_pPlayer{};
	};
}
