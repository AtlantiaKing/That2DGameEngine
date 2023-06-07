#pragma once

#include "State.h"

#include "glm/vec2.hpp"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class FygarRoamingState final : public State
	{
	public:
		FygarRoamingState(that::GameObject* pFygar, that::GameObject* pPlayer);
		virtual ~FygarRoamingState() = default;

		virtual std::unique_ptr<digdug::State> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

	private:
		void UpdateMovement();

		that::GameObject* m_pFygarObj{};
		that::GameObject* m_pPlayer{};

		glm::ivec2 m_Direction{ 1, 0 };

		float m_RoamTime{};
		float m_TimeUntilStateChange{};
		float m_AttackChance{ 0.8f };

		const float m_MinTimeUntilState{ 2.0f };
		const float m_MaxTimeUntilState{ 7.0f };
	};
}