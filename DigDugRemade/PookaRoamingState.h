#pragma once

#include "PookaState.h"

#include "glm/vec2.hpp"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class PookaRoamingState final : public PookaState
	{
	public:
		PookaRoamingState(that::GameObject* pPooka, that::GameObject* pPlayer);

		virtual std::unique_ptr<digdug::PookaState> Update() override;
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
