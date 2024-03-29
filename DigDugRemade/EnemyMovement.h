#pragma once

#include "Component.h"

#include "glm/vec2.hpp"

namespace digdug
{
	class EnemyMovement final : public that::Component
	{
	public:
		EnemyMovement() = default;
		virtual ~EnemyMovement() = default;

		EnemyMovement(const EnemyMovement& other) = delete;
		EnemyMovement(EnemyMovement&& other) = delete;
		EnemyMovement& operator=(const EnemyMovement& other) = delete;
		EnemyMovement& operator=(EnemyMovement&& other) = delete;

		virtual void Update() override;
	private:
		glm::ivec2 m_Direction{ 1,0 };
		int m_MinX{ 1 };
		int m_MaxX{ 10 };
	};
}
