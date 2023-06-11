#pragma once

#include "FygarLogic.h"

#include "glm/vec2.hpp"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class FygarRoamingAI final : public FygarLogic
	{
	public:
		FygarRoamingAI(that::GameObject* pFygar);
		virtual ~FygarRoamingAI() = default;

		virtual std::unique_ptr<EnemyState> Update() override;
	private:
		void UpdateMovement();

		that::GameObject* m_pFygarObj{};

		glm::ivec2 m_Direction{};

		float m_RoamTime{};
		float m_TimeUntilStateChange{};
		float m_AttackChance{ 0.7f };

		const float m_MinTimeUntilState{ 2.0f };
		const float m_MaxTimeUntilState{ 4.0f };
	};
}
