#pragma once

#include "State.h"

#include "glm/vec2.hpp"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class PookaRoamingState final : public State
	{
	public:
		PookaRoamingState(that::GameObject* pPooka, that::GameObject* pPlayer);
		virtual ~PookaRoamingState() = default;

		virtual std::unique_ptr<digdug::State> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

	private:
		void UpdateMovement();

		that::GameObject* m_pPookaObj{};
		that::GameObject* m_pPlayer{};

		glm::ivec2 m_Direction{ 1, 0 };

		float m_RoamTime{};
		float m_TimeUntilGhost{};

		const float m_MinTimeUntilGhost{ 10.0f };
		const float m_MaxTimeUntilGhost{ 30.0f };
	};
}
